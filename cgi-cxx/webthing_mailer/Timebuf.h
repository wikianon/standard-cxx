/*
WebThing Source code: copyright WebThing Ltd, 1997.

All distribution, modification and use of this software is subject
to the terms and conditions described in the accompanying LICENSE file,
which also tells you how to obtain SUPPORT and WARRANTY.

The software as distributed carries NO WARRANTY.  Please see file
COPYING for details.

Further information and updates are available from
<URL:http://www.webthing.com/software/>
*/

#ifndef TIMEBUF_H
#define TIMEBUF_H

/* A hack for representing and manipulating time/date, using time_t
   It assumes that sizeof(time_t) will increase before the year 2038
   so that applications using Timebuf need only be recompiled (not
   rewritten) to avoid any possible Y2K-like problem.

   Timebuf now ;		// initialises to NOW
   Timebuf (time_t secs) ;
   Timebuf(yy,mm,dd,hh,mm,ss) ;	// last three are optional

   cout << timebuf prints it in Internet date field format suitable
	for HTTP headers
   Timebuf::y4m2d2(char) prints YYYY-MM-DD (with your choice of delimiter)

   add**(int n) increments ** (**=Y, M, W, D, H, M or S) by n

   and there are a couple of convenient handles

*/
#include <iostream.h>
#include <strstream.h>
#include <string.h>
#include <time.h>

class Timebuf {
	time_t _secs ;
	struct tm _tm ;
	char _day[4] ;
	char _month[4] ;
	char _year[5] ;
	char _date[3] ;
	char _time[9] ;
	inline void set() ;
	inline void set(struct tm* tm) ;
public:
	inline void set(int, const int, const int,
		const int =12, const int =0, const int =0) ;
	Timebuf(time_t secs) : _secs(secs) { set() ; }
	Timebuf() : _secs(::time(0)) { set() ; }
	Timebuf(int yy, const int mm, const int dd,
		const int h =12, const int m =0, const int s =0)
		: _secs(::time(0)) { set() ; set(yy,mm,dd,h,m,s) ; }
	const char* day() const { return _day ; }
	const char* month() const { return _month ; }
	const char* time() const { return _time ; }
	const char* date() const { return _date ; }
	const char* year() const { return _year ; }
	inline const char* y4m2d2(const char delim ='/') const ;
	const int dd() const { return _tm.tm_mday ; }
	const int mm() const { return _tm.tm_mon ; }
	const int yy() const { return _tm.tm_year ; }
	const int wd() const { return _tm.tm_wday ; }

	Timebuf& addss(const int dt) { _secs += dt ; set() ; return *this ; }
	Timebuf& addmm(const int dt) { return addss(60*dt) ; }
	Timebuf& addhh(const int dt) { return addss(3600*dt) ; }
	Timebuf& adddd(const int dt) { return addss(86400*dt) ; }
	Timebuf& addww(const int dt) { return addss(604800*dt) ; }
	Timebuf& addYY(const int dt) { _tm.tm_year += dt ; set(&_tm) ; return *this ; }
	Timebuf& addMM(const int dt) {
		_tm.tm_mon += dt ;
		while ( _tm.tm_mon >= 12) {
			_tm.tm_mon -= 12 ;
			++_tm.tm_year ;
		}
		while ( _tm.tm_mon < 0) {
			_tm.tm_mon += 12 ;
			--_tm.tm_year ;
		}
		set(&_tm) ;
		return *this ;
	}
} ;
  
inline ostream& operator<<(ostream& o, const Timebuf& t) {
  return o
	<< t.day() << ", " << t.date() << ' ' << t.month()
	<< ' ' << t.year() << ' ' << t.time() << " GMT" ;
}
inline void Timebuf::set() {
  memcpy(&_tm, gmtime(&_secs), sizeof(struct tm)) ;
  set(&_tm) ;
}
inline void Timebuf::set(struct tm* tm) {
//  _tm = tm ;
  char* tbuf = asctime(tm) ;
  //_secs = ::time(_tm) ;
  istrstream(tbuf,strlen(tbuf))
	>> _day >> _month >> _date >> _time >> _year ;
}
inline const char* Timebuf::y4m2d2(const char delim) const {
  ostrstream ret ;
  ret << _year << delim << mm()+1 << delim << dd() << ends ;
  return ret.str() ;
}
inline void Timebuf::set(int yy, const int mm, const int dd,
			const int h, const int m, const int s) {
  yy -= 1900 ;
  _tm.tm_year = yy ;
  _tm.tm_mon = mm ;
  _tm.tm_mday = dd ;
  _tm.tm_hour = h ;
  _tm.tm_min = m ;
  _tm.tm_sec = s ;
  _secs = mktime(&_tm) ;
  set() ;
}
#endif
