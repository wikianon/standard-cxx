/****************************************************************************
http://www.shoup.net/ntl/doc/SmartPtr.cpp.html
SmartPtr: a smart pointer class.

Synopsis: provides a reference counted smart pointer, similar to shared_ptr
in the standard library.  It is provided here to minimize reliance
on the standard library, especially for older C++ compilers, which may
not provide shared_ptr, or it may be in TR1, which gets messy.


Examples:


  SmartPtr<T> p1;         // initialize to null
  SmartPtr<T> p1(0); 

  SmartPtr<T> p2 = 0;     // 0/nullptr implicitly converts to SmartPtr<T>

  SmartPtr<T> p3(p1);     // copy constructor

  T *rp;
  SmartPtr<T> p4(rp);     // construct using raw pointer (explicit): better 
                          // to use MakeSmart below

  p1 = MakeSmart<T>(...); // build new T object by invoking constructor
                          // T(...) with pseudo-variadic templates.
                          // This is safer and more efficient that
                          // using the raw-pointer constructor
                        
  p1 = p2;                // assignment
  p1 = 0;                 // assign null


  if (!p1) ...            //  test for null
  if (p1 == 0) ... 

  if (p1) ...             // test for not null ... 
  if (p1 != 0) ... 

  if (p1 == p2) ...       // test for equality 
  if (p1 != p2) 

  *p1                     // dereferencing
  p1->...

  p1.get();               // return the underlying raw pointer...dangerous!

  p1.swap(p2);            // fast swap
  swap(p1, p2);


Automatic Conversions:

If S is another class, SmartPtr<S> converts to SmartPtr<T> if S* converts to T*
(for example, if S is a subclass of T).  Similarly, SmartPtr<S> and SmartPtr<T>
may be compared if S* and T* may be compared.

0/nullptr automatically converts to SmartPtr<T>.

MakeSmart:

One can write SmartPtr<T> p = MakeSmart<T>(x1, ..., xn), and this will create a
smart pointer to an object constructed as T(x1, ..., xn).  Besides notational
convenience, it also reduces the number of memory allocations from 2 to 1, as
the data and control block can be allocated in one chunck of memory.

This is implemented without reliance on C++11 features, which means that there
are limitations.  First, the number n of arguments is limited to 9.  And
second, all arguments are pass by const reference. However, you can work around
this by using the helper function Fwd.  For example, if T has a 2-argument
constructor where the second must be a non-const reference of some type, and x2
is a variable of that type, you can write MakeSmart<T>(x1, Fwd(x2)), to forward
that reference through all the template nonsense in a typesafe manner.

MakeRaw:

One can also write T *p = MakeRaw<T>(x1, ..., xn) to create a 
raw pointer.  This is the same as writing T *p = new T(x1, ..., xn),
except that error handling is determined by the NTL_EXCEPTION
flag (on => bad_alloc exception is thrown, off => error message
and abort).

MakeRawArray:

Another utility routine: one can write T *p = MakeRawArray<T>(n)
to make a plain array of n T objects.  Error handling is the same
as for MakeRaw.

Dynamic casting:

I've also supplied a dynamic cast operation for smart pointers.

   SmartPtr<Derived> d = MakeSmart<Derived>(); // d points to Derived
   SmartPtr<Base> b = d; // implicit upcast: OK

   SmartPtr<Derived> d1 = DynamicCast<Derived>(b);
      // downcast to a Derived object -- returns null for a bad cast

DeleterPolicy:

Normally, when the object pointed to a SmartPtr needs to be destroyed, this is
done by invoking delete on the raw pointer.  The user can override this
behavior by specifying a "deleter policy", which is a class P that defines a
static member function deleter, which is invoked as P::deleter(p).  Such a
policy can be attached to a SmartPtr using a specialized constructor (see
below).

A deleter policy can be useful, for example, in realizing the PIPL
pattern, where the class T's definition is not visible.  The specified deleter
can invoke a free-standing function that itself invokes delete.  A deleter
policy can also be useful is memory is to be managed using some mechanism other
than new/delete.


Implementation notes:

If NTL is compiled with the NTL_THREADS option, then the reference counting
will be thread safe.

The SmartPtrControl class heirarchy is used to make sure the right destructor
is called when the ref count goes to zero.  This can be an issue for forward
declared classes and for subclasses.  For example, if T is forward declared in
a context where the ref count goes to zero, or if the object's actual type is a
subclass of T and T's destructor was not declared virtual.  The implementation
of SmartPtr guarantees correct behavior in these situations.

The null tests p, !p, p == 0, are all effected via an implicit conversion from
SmartPtr<T> to a funny pointer type (a pointer to a member function, which
avoids other, unwanted implicit conversions: this is the so-called "safe bool
idiom");

Also, there is an implicit conversion from another funny pointer type to
SmartPtr<T>, which is how the implicit conversion from 0/nullptr is achieved.

In C++11 both of the above effects could perhaps be achieved more directly.
The new "explict bool" operator can replace the "safe bool idiom", and 
the new nullptr_t type could be used to get the conversion from null to work.

NOTES: See http://www.artima.com/cppsource/safebool.html for more on the "safe
bool idiom".  


*****************************************************************************/

// The default "deleter policy"
struct DefaultDeleterPolicy {

   template<class T>
   static void deleter(T *p) { delete p; }

};

// A tagging class, for better readability in invoking constructor.
// Usage: SmartPtr<T> p(r, ChoosePolicy<MyDeleterPolicy>());
template<class P>
struct ChoosePolicy { };





template<class T>
class SmartPtr {
public:
public:
   template<class Y> explicit SmartPtr(Y* p);
   // construct smart pointer from raw pointer with deleter policy
   // DefaultDeleterPolicy (so p should be allocated using new).

   // NOTE: Y* must convert to T*, but upon the original pointer is preserved
   // so that when ref count drops to 0, the *original* object of type Y is destroyed.

   // EXCEPTIONS: a control block is dynamically allocated;
   //    if this allocation fails, the object pointed to by p is destroyed
   //    and a bad_alloc exception is thrown

   template<class Y, class P> SmartPtr(Y* p, ChoosePolicy<P>);
   // construct smart pointer from raw pointer with deleter policy P.

   // NOTE: Y* must convert to T*, but upon the original pointer is preserved
   // so that when ref count drops to 0, the *original* object of type Y is destroyed.

   // EXCEPTIONS: a control block is dynamically allocated;
   //    if this allocation fails, the object pointed to by p is destroyed
   //    and a bad_alloc exception is thrown

   SmartPtr();
   // initial value null

   SmartPtr(fake_null_type1);
   // automatic conversion from 0/nullptr

   ~SmartPtr();
   // destructor

   SmartPtr(const SmartPtr& other);
   SmartPtr& operator=(const SmartPtr& other);
   // copy and assignment

   template<class Y> SmartPtr(const SmartPtr<Y>& other);
   template<class Y> SmartPtr& operator=(const SmartPtr<Y>& other);
   // copy and assignment

   SmartPtr(SmartPtr&& other) noexcept;
   SmartPtr& operator=(SmartPtr&& other) noexcept;
   // move semantics (C++11 only)

   template<class Y> SmartPtr(SmartPtr<Y>&& other) noexcept;
   template<class Y> SmartPtr& operator=(SmartPtr<Y>&& other);
   // move semantics (C++11 only)

   T& operator*()  const;
   T* operator->() const;
   // indirection

   T* get() const;
   // get underlying raw pointer

   void swap(SmartPtr& other);

   SmartPtr(fake_null_type);
   // allows assignment and initialization from 0

   operator fake_null_type() const;
   // allows comparisons to 0

   template<class Y> SmartPtr<Y> DynamicCast() const;
};


// free swap function
template<class T> void swap(SmartPtr<T>& p, SmartPtr<T>& q);

// free dynamic cast function
template<class X, class Y> SmartPtr<X> DynamicCast(const SmartPtr<Y>& p);


// Equality testing
template<class X, class Y>
bool operator==(const SmartPtr<X>& a, const SmartPtr<Y>& b);

template<class X, class Y>
bool operator!=(const SmartPtr<X>& a, const SmartPtr<Y>& b);

// MakeSmart psuedo-variadic template
template<class T, class X1, ..., class Xn>
SmartPtr<T> MakeSmart(const X1& x1, ..., const Xn& xn);
// EXCEPTIONS: may throw if dynamic constrction of T(x1, ..., xn) fails


// EXCEPTIONS: unless otherwise specified, the methods above
// never throw an exception (under C++11 rules, if a destructor
// is invoked that throws an exception, the program will terminate).


/****************************************************************************

Experimantal: CloneablePtr<T> ...essentially same interface as SmartPtr, but 
allows cloning of complete objects.  The differences:
*  must construct using MakeCloneable
*  a clone method is provided
*  implicit conversion from CloneablePtr to SmartPtr is allowed

Example:

   CloneablePtr<Derived> d = MakeCloneable<Derived>(); 
   // d points to Derived

   CloneablePtr<Base> b = d; // implicit upcast: OK

   CloneablePtr<Base> b1 = b.clone(); 
   // clone of b, which is really a Derived object

   CloneablePtr<Derived> d1 = DynamicCast<Derived>(b1);
   // downcast to a Derived object -- returns null for a bad cast

   SmartPtr<Base> b2 = d1;
   


Implementation:

In the clone method, the object is constructed using the copy constructor for
the type T, where T is the compile-time type with which the first smart pointer
to this object was was created, even if the pointer has been subsequently
upcasted to a base type S.  Such objects must have been initially created using
the MakeCloneable function.  It turns out, this is hard to do in a completely
standards-compliant way, because of the type erasure going on.  So I settled on
the current method, which does some low-level pointer arithmetic.  Even with
fancy things like multiple and virtual inheritance, it should work, under the
assumption that if two objects have the same (runtime) type, then their memory
layout is the same.  I don't think anything like that is guaranteed by the
standard, but this seems reasonable, and it seems to work.  Like I said, it is
experimental, and I would appreciate feedback from C++ gurus.

Note that NTL does not use this feature, but I do have applications where this
is convenient.


**********************************************************************************/


template<class T>
class CloneablePtr {
public:
   CloneablePtr();
   // initial value null

   ~CloneablePtr();
   // if ref count drops to zero, then delete referent

   CloneablePtr(const CloneablePtr& other);
   CloneablePtr& operator=(const CloneablePtr& other);
   // copy and assignment

   template<class Y> CloneablePtr(const CloneablePtr<Y>& other);
   template<class Y> CloneablePtr& operator=(const CloneablePtr<Y>& other);
   // copy and assignment

   CloneablePtr(CloneablePtr&& other) noexcept;
   CloneablePtr& operator=(CloneablePtr&& other) noexcept;
   // move semantics (C++11 only)

   template<class Y> CloneablePtr(CloneablePtr<Y>&& other) noexcept;
   template<class Y> CloneablePtr& operator=(CloneablePtr<Y>&& other);
   // move semantics (C++11 only)

   T& operator*()  const;
   T* operator->() const;
   // indirection

   T* get() const;
   // get underlying raw pointer

   void swap(CloneablePtr& other);

   CloneablePtr(fake_null_type);
   // allows assignment and initialization from 0

   operator fake_null_type() const;
   // allows comparisons to 0

   template<class Y> CloneablePtr<Y> DynamicCast() const;

   CloneablePtr clone() const;
   // construct a clone, using the copy constructor
   // EXCEPTIONS: may throw if copy construction fails


   template<class Y> operator SmartPtr<Y>();
   // implicit conversion from CloneablePtr<T> to SmartPtr<Y>,
   // allowed if T* converts implicitly to Y*.
};


// free swap function
template<class T> void swap(CloneablePtr<T>& p, CloneablePtr<T>& q);

// free dynamic cast function
template<class X, class Y> CloneablePtr<X> DynamicCast(const CloneablePtr<Y>& p);


// Equality testing
template<class X, class Y>
bool operator==(const CloneablePtr<X>& a, const CloneablePtr<Y>& b);

template<class X, class Y>
bool operator!=(const CloneablePtr<X>& a, const CloneablePtr<Y>& b);

// MakeCloneable psuedo-variadic template
template<class T, class X1, ..., class Xn>
CloneablePtr<T> MakeCloneable(const X1& x1, ..., const Xn& xn);
// EXCEPTIONS: may throw if dynamic constrction of T(x1, ..., xn) fails


// EXCEPTIONS: unless otherwise specified, the methods above
// never throw an exception (under C++11 rules, if a destructor
// is invoked that throws an exception, the program will terminate).






/**********************************************************************

UniquePtr<T> -- unique pointer to object with copying disabled.
Useful for pointers inside classes so that we can
automatically destruct them.  

Constructors:
   UniquePtr<T> p1;     // initialize with null
   UniquePtr<T> p1(0); 

   T* rp;
   UniquePtr<T> p1(rp); // construct using raw pointer (explicit)

   p1 = 0;              // destroy's p1's referent and assigns null

   p1.make(...);        // destroy's p1's referent and assigns
                        // a fresh objected constructed via T(...),
                        // using psuedo-variadic templates
                
   p1.reset(rp);        // destroy's p1's referent and assign rp

   if (!p1) ...         // test for null
   if (p1 == 0) ...

   if (p1) ...          // test for nonnull
   if (p1 != 0) ...

   if (p1 == p2) ...    // test for equality
   if (p1 != p2) ...   

   *p1                  // dereferencing
   p1->...


   rp = p1.get();       // fetch raw pointer
   rp = p1.release();   // fetch raw pointer, and set to null

   p1.move(p2);         // move p2 to p1, destroying p1's referent
                        //   if p1 != p2

   p1.swap(p2);         // swap pointers
   swap(p1, p2);


DeleterPolicy:

UniquePtr supports a "deleter policy", analogous to that used in SmartPtr.

Normally, when the object pointed to a UniquePtr needs to be destroyed, this is
done by invoking delete on the raw pointer.  The user can override this
behavior by specifying a "deleter policy", which is a class P that defines a
static member function deleter, which is invoked as P::deleter(p).  

Unlike with a SmartPtr, the deleter policy must be attached to the type.
The default policy is the same DefaultDeleterPolicy, defined above.

A deleter policy can be useful, for example, in realizing the PIPL
pattern, where the class T's definition is not visible.  The specified deleter
can invoke a free-standing function that itself invokes delete.  A deleter
policy can also be useful is memory is to be managed using some mechanism other
than new/delete.

   
**********************************************************************/


template<class T, class P=DefaultDeleterPolicy>
class UniquePtr {
public:
   explicit UniquePtr(T *p);
   // construct UniquePtr from raw pointer (allocated with new)

   UniquePtr();
   // initial value is null

   UniquePtr(UniquePtr&& other) noexcept;
   UniquePtr& operator=(UniquePtr&& other) noexcept;
   // move semantics (C++11 only)

   UniquePtr& operator=(fake_null_type1);
   // allows assignment of 0; equivalent to calling reset()

   ~UniquePtr();
   // destroys referent by calling P::deleter

   void reset(T* p = 0);
   // reset underlying pointer to p, destroying original referent
   // by calling P::deleter

   template<class T, class X1, ..., class Xn>
   void make(const X1& x1, ..., const Xn& xn);
   // pseudo-variadic template, roughly equivalent to
   // reset(new T(x1, ..., xn))
   // EXCEPTIONS: this may throw (but provides strong ES guarantee)

   T& operator*()  const;
   T* operator->() const;
   // indirection

   T* get() const;
   // get raw pointer

   T* release();
   // returns raw pointer, and sets the raw pointer to null

   void move(UniquePtr& other);
   // move other to *this, destroying original referent
   // by calling P::deleter

   void swap(UniquePtr& other);
   // swap raw pointers

   operator fake_null_type() const;
   // allows comparison with 0

private:
   UniquePtr(const UniquePtr&); // disabled
   void operator=(const UniquePtr&); // disabled
};


// free swap function
template<class T> void swap(UniquePtr<T>& p, UniquePtr<T>& q);



// Equality testing

template<class X, class P> bool operator==(const UniquePtr<X,P>& a, const UniquePtr<X,P>& b);
template<class X, class P> bool operator!=(const UniquePtr<X,P>& a, const UniquePtr<X,P>& b);


// EXCEPTIONS: unless otherwise specified, the methods above
// never throw an exception (under C++11 rules, if a destructor
// is invoked that throws an exception, the program will terminate).


/**********************************************************************

CopiedPtr<T> -- essentially the same interface and implemetation as UniquePtr,
with the following exceptions:

 * copy constructor is defined: by default, it will create a copy
   of the referrent using T's copy constructor (but this bahavior
   can be overridden -- see below)

 * assignment operator is defined (and implemented in terms of the
   copy constructor)

 * The policy managing a CopiedPtr specifier deleter and copier functions:
   the deleter is used to delete objects and the copies is used for making
   copies (see below).

NOTE: this class is meant to replace the OptionalVal class, whose
interface is not so nice.  For backwards compatibility, OptionalVal will
be maintained, however.
   
**********************************************************************/


// This class specifies the default copier
struct DefaultCopierPolicy {

   template<class T>
   static T* copier(T *p) { return (p ?  MakeRaw<T>(*p) : 0); }

};

// This class specifies an alternative copier, which is meant
// to perform "deep" copies on class heirarchies that support an
// appropriate clone() method.
struct CloningCopier {

   template<class T>
   static T* copier(T *p) { return (p ?  p->clone() : 0); }

};

struct DefaultCopiedPtrPolicy : DefaultDeleterPolicy, DefaultCopierPolicy { };
struct CloningCopiedPtrPolicy : DefaultDeleterPolicy, CloningCopier { };



template<class T, class P=DefaultCopiedPtrPolicy>
class CopiedPtr {
public:
   explicit CopiedPtr(T *p);
   // construct CopiedPtr from raw pointer (allocated with new)

   CopiedPtr();
   // initial value is null

   CopiedPtr(const CopiedPtr& other);
   // creates a copy of other's referent by calling P::copier

   void operator=(const CopiedPtr&);
   // creates a copy of other's referent by calling P::copier,
   // and destroys original referent by calling P::deleter

   CopiedPtr& operator=(fake_null_type1);
   // allows assignment of 0; equivalent to calling reset()

   ~CopiedPtr();
   // destroys referent by calling P::deleter

   CopiedPtr(CopiedPtr&& other) noexcept;
   CopiedPtr& operator=(CopiedPtr&& other) noexcept;
   // move semantics (C++11 only)

   void reset(T* p = 0);
   // reset underlying pointer to p, destroying original referent
   // by calling P::deleter

   template<class T, class X1, ..., class Xn>
   void make(const X1& x1, ..., const Xn& xn);
   // pseudo-variadic template, roughly equivalent to
   // reset(new T(x1, ..., xn))
   // EXCEPTIONS: this may throw (but provides strong ES guarantee)

   T& operator*()  const;
   T* operator->() const;
   // indirection

   T* get() const;
   // get raw pointer

   T* release();
   // returns raw pointer, and sets the raw pointer to null

   void move(CopiedPtr& other);
   // move other to *this, destroying original referent
   // by calling P::deleter


   void swap(CopiedPtr& other);
   // swap raw pointers

   operator fake_null_type() const;
   // allows comparison with 0

};


// free swap function
template<class T> void swap(CopiedPtr<T>& p, CopiedPtr<T>& q);



// Equality testing

template<class X, class P> bool operator==(const CopiedPtr<X,P>& a, const CopiedPtr<X,P>& b);
template<class X, class P> bool operator!=(const CopiedPtr<X,P>& a, const CopiedPtr<X,P>& b);


// EXCEPTIONS: unless otherwise specified, the methods above
// never throw an exception (under C++11 rules, if a destructor
// is invoked that throws an exception, the program will terminate).




/**********************************************************************

UniqueArray<T> -- similar to UniquePtr, but for arrays.  These arrays cannot be
resized -- for that, you should use the Vec class.

Constructors:
   UniqueArray<T> p1;     // initialize with null
   UniqueArray<T> p1(0); 

   T* rp;
   UniqueArray<T> p1(rp); // construct using raw pointer (explicit)

   p1 = 0;              // destroy's p1's referent and assigns null

   p1.SetLength(n);     // destroy's p1's referent and assigns
                        // a fresh objected constructed via new T[n]
                
   p1.reset(rp);        // destroy's p1's referent and assign rp

   if (!p1) ...         // test for null
   if (p1 == 0) ...

   if (p1) ...          // test for nonnull
   if (p1 != 0) ...

   if (p1 == p2) ...    // test for equality
   if (p1 != p2) ...   

   p1[i]                // array indexing

   rp = p1.get();       // fetch raw pointer
   rp = p1.release();   // fetch raw pointer, and set to null
   p1.move(p2);         // move p2 to p1, destroying p1's referent 
                        //   if p1 != p2

   p1.swap(p2);         // fast swap
   swap(p1, p2);

   
**********************************************************************/


template<class T>
class UniqueArray {
public:
   explicit UniqueArray(T *p);
   // construct from raw pointer (allocated with new[])

   UniqueArray();
   // initially null

   UniqueArray& operator=(fake_null_type1);
   // allows of 0

   ~UniqueArray();

   UniqueArray(UniqueArray&& other) noexcept;
   UniqueArray& operator=(UniqueArray&& other) noexcept;
   // move semantics (C++11 only)

   void reset(T* p = 0);
   // reset with raw pointer, destroying referent

   void SetLength(long n);
   // destroys referent and allocates an array of size n
   // EXCEPTIONS: this may throw (but provides strong ES guarantee)

   T& operator[](long i) const;
   // accesses ith element in the array (currently no range checking)

   T* get() const;
   // get raw pointer

   T* elts() const;
   // get raw pointer (for compatibility with the Vec class)

   T* release();
   // get raw pointer and reset to null

   void move(UniqueArray& other);
   // move raw pointer

   void swap(UniqueArray& other);
   // swap raw pointer

   operator fake_null_type() const;
   // allows comparison to 0

private:
   UniqueArray(const UniqueArray&); // disabled
   void operator=(const UniqueArray&); // disabled

};



// free swap function
template<class T> void swap(UniqueArray<T>& p, UniqueArray<T>& q);



// Equality testing

template<class X> bool operator==(const UniqueArray<X>& a, const UniqueArray<X>& b);
template<class X> bool operator!=(const UniqueArray<X>& a, const UniqueArray<X>& b);




/**********************************************************************

Unique2DArray<T> -- unique pointer to array of arrays.

This is very similar to UniqueArray< UniqueArray<T> >, except that 
we can retrofit old code that accepts objects of type T**.

Constructors:
   Unique2DArray<T> p1;     // initialize with null
   Unique2DArray<T> p1(0); 

   p1 = 0;              // destroy's p1's referent and assigns null
   p1.reset();

   p1.SetLength(n);     // destroy's p1's referent and assigns
                        // a fresh array of null pointers

   p1.SetDims(n, m)     // creates an n x m array
                
   if (!p1) ...         // test for null
   if (p1 == 0) ...

   if (p1) ...          // test for nonnull
   if (p1 != 0) ...

   if (p1 == p2) ...    // test for equality
   if (p1 != p2) ...   

   p1[i]                // array indexing

   T **rp;
   rp = p1.get();       // fetch raw pointer
   rp = p1.release();   // fetch raw pointer, and set to null
   p1.move(p2);         // if p1 != p2 then:
                        //    makes p1 point to p2's referent,
                        //    setting p2 to null and destroying
                        //    p1's referent

   p1.swap(p2);         // fast swap
   swap(p1, p2);

   
**********************************************************************/


template<class T>
class Unique2DArray {
public:
   typedef T *T_ptr;

   Unique2DArray();
   // initially null

   Unique2DArray& operator=(fake_null_type1);
   // allows initialization and assignment of 0

   ~Unique2DArray();
   // destroys the entire array and each row in the array

   Unique2DArray(Unique2DArray&& other) noexcept;
   Unique2DArray& operator=(Unique2DArray&& other) noexcept;
   // move semantics (C++11 only)

   void reset();
   // reset to null


   void SetLength(long n);
   // resets the array to a vector of length n,
   // each entry initialized to null.
   // EXCEPTIONS: may throw (provides strong ES guarantee)

   void SetDims(long n, long m);
   // resets the array to a 2D array with n rows and m columns.
   // EXCEPTIONS: may throw (provides strong ES guarantee)

   void SetDimsFrom1(long n, long m);
   // same as above, but only initializes rows 1..n-1.
   // this helps with some legacy code.
   // EXCEPTIONS: may throw (provides strong ES guarantee)

   T_ptr& operator[](long i) const;
   // array indexing, no range checking

   T_ptr* get() const;
   // return underlying pointer

   T_ptr* release() { len = 0; return dp.release(); }
   // return underlying pointer and reset to null


   void move(Unique2DArray& other);
   // move pointers

   void swap(Unique2DArray& other);
   // swap pointers

   operator fake_null_type() const;
   // allows comparison to 0


private:

   Unique2DArray(const Unique2DArray&); // disabled
   void operator=(const Unique2DArray&); // disabled

};


// free swap function
template<class T> void swap(Unique2DArray<T>& p, Unique2DArray<T>& q);



// Equality testing

template<class X> bool operator==(const Unique2DArray<X>& a, const Unique2DArray<X>& b);
template<class X> bool operator!=(const Unique2DArray<X>& a, const Unique2DArray<X>& b);





/**********************************************************************


OptionalVal<T> -- unique pointer to object with copying enabled.

NOTE: this class is deprecated; use CopiedPtr instead.
It will, however, be maintained indefinitely for backward compatibility.

Constructors:
   OptionalVal<T> p1;     // initialize with null

   T* rp;
   OptionalVal<T> p1(rp); // construct using raw pointer (explicit)

   OptionalVal<T> p2(p1); // construct a copy of p1's referent

    

   p1.make(...);        // destroy's p1's referent and assigns
                        // a fresh objected constructed via T(...),
                        // using psuedo variadic templates
                
   p1.reset(rp);        // destroy's p1's referent and assign rp

   if (p1.exists()) ... // test for null

   p1.val()             // dereference

   rp = p1.get();       // fetch raw pointer
   rp = p1.release();   // fetch raw pointer, and set to NULL
   p1.move(p2);         // move p2 to p1, destroying p1's referent
                        //   if p1 != p2

   p1 = p2;             // deep copy, using T's copy constructor

   p1.swap(p2);         // swap pointers
   swap(p1, p2);

   
**********************************************************************/


template<class T>
class OptionalVal{
public:
   explicit OptionalVal(T *p);
   // initialize using raw pointer (allocated with new)

   OptionalVal();
   // initialize to null

   OptionalVal(const OptionalVal& other);
   // initialize using a deep copy (via T's copy constructor)

   OptionalVal& operator=(const OptionalVal& other);
   // assignment using a deep copy (via T's copy constructor)

   ~OptionalVal();
   // destroys the referent

   OptionalVal(OptionalVal&& other) noexcept;
   OptionalVal& operator=(OptionalVal&& other) noexcept;
   // move semantics (C++11 only)

   void reset(T* p = 0);
   // resets the referent

   template<class T, class X1, ..., class Xn>
   void make(const X1& x1, ..., const Xn& xn);
   // pseudo-variadic template.
   // resets the referent to a new object T(x1, ..., xn)
   // EXCEPTIONS: may throw an exception (but provides strong ES guarantee)

   T& val() const;
   // returns reference to referent 
   // if underlying pointer p is null, the indirection *p
   // is undefined behavior, but most likely leads to program termination

   bool exists() const;
   // checks that underlying pointer is not null

   T* get() const;
   // returns underlying raw pointer

   T* release();
   // returns raw pointer, and sets the raw pointer to null

   void move(OptionalVal& other);
   // performs a (shallow) pointer move

   void swap(OptionalVal& other);
   // performs a (shallow) pointer swap

};


// free swap function
template<class T> void swap(OptionalVal<T>& p, OptionalVal<T>& q);



// EXCEPTIONS: unless otherwise specified, the methods above
// never throw an exception (under C++11 rules, if a destructor
// is invoked that throws an exception, the program will terminate).