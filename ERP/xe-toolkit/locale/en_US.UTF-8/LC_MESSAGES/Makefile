MO = mdr-messages.mo mdr-explanations.mo mdr-names.mo
.SUFFIXES: .mo .po

.po.mo:
	msgfmt -o $*.mo $*.po

all:	$(MO)

clean:
	/bin/rm -f $(MO)
