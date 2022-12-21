// ================================================================================
//
// Copyright: M.Nelson - technische Informatik
//            Die Software darf unter den Bedingungen 
//            der APGL ( Affero Gnu Public Licence ) genutzt werden
//            
//    weblet: personnal/time/day
// ================================================================================
{
  var i;
  var str = "";
  var obj;

  weblet.loadClass("MneAjaxTable", "table/mne_atable.js");

  var ivalues = 
  {

      styleName : 'style.css',
      schema    : 'mne_personnal',
      table     : 'time',

      slotstartschema : 'mne_personnal',
      slotstarttable  : 'timemanagement_param',

      personquery : 'timeday',
      personcols  : 'needtime,personid,loginname,fullname,wtime,dayname,day,duration',
      personcolstyle : ',,,,,mne_personal_dayname,,bold',
      personrowstyle: 'result',
      personrowstylecol : '0',

      timeschema      : 'mne_personnal',
      timequery       : 'time',
      timecols        : "orderid,ordernumber,productnumber,stepdescription,clocktime,duration,comment,refname,orderdescription,productname",
      timecolstyle    : ",tedit,tedit,tedit rdonly,tedit,tedit,tedit,,,",
      timescols       : "start",

      orderschema : 'mne_crm',
      orderquery  : 'order_detail',

      orderproducttimeschema : 'mne_personnal',
      orderproducttimequery  : 'orderproducttime_skill',
      orderproducttimecheckquery  : 'orderproducttime',

      okschema    : 'mne_personnal',
      okfunction  : "time_mod",

      delschema   : 'mne_personnal',
      delfunction : "time_del",

      returntimeout : 0
  };

  var svalues = 
  {
  };

  weblet.initDefaults(ivalues, svalues);

  weblet.loadview();
  weblet.frame.position = "relative";
  weblet.eleMkClass(weblet.frame, 'mne_personal_time_day');

  weblet.obj.timeids = ("timeid,orderproducttimeid," + weblet.initpar.timecols).split(',');
  weblet.obj.editable = (",," + weblet.initpar.timecolstyle).split(',');


  weblet.keydown = function(evt, inside)
  {
    if ( inside == true || window.mneDocevents.checkInside(evt, this.origframe) )
    {
      if ( evt.keyCode == 39 )
      {
        this.next();
        this.doceventstop = true;
        return false;
      }
      else if ( evt.keyCode == 37 )
      {
        this.prev();
        this.doceventstop = true;
        return false;
      }
      else
      {
        return MneAjaxWeblet.prototype.keydown.call(this, evt, true);
      }
    }
    
    return true;
  }
  
  for ( i=0; i<weblet.obj.timeids.length; i++ )
  {
    var ele = weblet.win.document.createElement("input");

    ele.id = weblet.obj.timeids[i] + "Input";

    ele.colnum = i;
    ele.editable = weblet.obj.editable[i].indexOf('tedit') != -1;
    ele.onreturn = function()
    {
      try
      {
        var result = this.weblet.ok('return');
        {
          var w = this.weblet;
          var timeout = function() { w.find_empty(); };
          if ( result != true ) window.setTimeout(timeout, 0);
        }
        return result;
      }
      catch (e)
      {
        this.weblet.exception("", e);
        return true;
      }
    };
    
    ele.onkeydown = function(e)
    {
      var evt = MneDocEvents.prototype.mkEvent(this.weblet.win, e);

      if (evt.shiftKey != true && evt.keyCode == 9)
      {
        this.weblet.ontabulator(this);
      }
      if (evt.shiftKey == true && evt.keyCode == 9)
      {
        this.weblet.onbacktabulator(this);
      }
      if ( evt.keyCode == 13 )
      {
        this.onreturn();
      }
      else
      {
        this.focus();
      }

      if ( evt.keyCode == 9 || evt.keyCode == 13 )
      {
        if ( MneMisc.prototype.navigator != "MSIE" )
        {
          e.preventDefault();
        }
        else
        {
          window.event.returnValue = false;
          window.event.cancelBubble = true;
        }
        return false;
      }
    }
    weblet.eleMkClass(ele, weblet.obj.editable[i], true);
    ele.openallways = weblet.obj.editable[i].indexOf("openallways") != -1;

    if ( weblet.obj.editable[i].indexOf("rdonly") != -1 )
    {
      ele.onkeydown = function(e)
      {
        var evt = MneDocEvents.prototype.mkEvent(this.weblet.win, e);
        if (evt.shiftKey != true && evt.keyCode == 9)
        {
          this.weblet.ontabulator(this);
        }
        if (evt.shiftKey == true && evt.keyCode == 9)
        {
          this.weblet.onbacktabulator(this);
        }
        else if ( evt.keyCode == 13 )
        {
          this.onreturn();
        }

        e.preventDefault();
        return false;
      }
      ele.rdonly = true;
    }
    weblet.frame.appendChild(ele);
  }

  weblet.obj.personids = weblet.initpar.personcols.split(',');
  for ( i=0; i<weblet.obj.personids.length; i++ )
  {
    if ( weblet.obj.personids[i] == 'day')
      weblet.obj.daycol = i;

    if ( weblet.obj.personids[i] == 'dayname')
      weblet.obj.daynamecol = i;
  }

  var attr = 
  {
      hinput : true,
      ordernumberInput        : { mykeyup : true, onkeyup : function(evt) { try { this.weblet.onkeyup(this,evt) } catch(e) { this.weblet.exception("Ordernumber", e) } } },
      productnumberInput      : { mykeyup : true, onkeyup : function(evt) { try { this.weblet.onkeyup(this, evt) } catch(e) { this.weblet.exception("Produktnumber", e) } } },
      orderproducttimeidInput : { checktype : weblet.inChecktype.notempty },
      login                   : { weblet : weblet, onkeyup : function(e) { var evt = MneDocEvents.prototype.mkEvent(this.weblet.win, e);  if ( evt.keyCode == 13 ) this.weblet.showValue(this.weblet); } },              
      clocktimeInput          : { inputtyp : "clocktime" },
      prevButton              : { onmousedown : function() { this.weblet.eleMkClass(this, 'docprevdown', true)}, onmouseup : function() { this.weblet.eleMkClass(this, 'docprevdown', false)}},
      nextButton              : { onmousedown : function() { this.weblet.eleMkClass(this, 'docnextdown', true)}, onmouseup : function() { this.weblet.eleMkClass(this, 'docnextdown', false)}},

  }

  if ( weblet.win.mne_config.username != 'admindb' ) attr['login'].remove = true;
  
  weblet.findIO(attr);
  weblet.obj.inputs.orderproducttimeid.inputcheckobject = weblet.obj.inputs.stepdescription;

  weblet.obj.inelements = {};
  for ( i in weblet.obj.inputs )
  {
    try 
    {
      if ( weblet.obj.inputs[i].parentNode == weblet.frame )
      {
        weblet.obj.inelements[i] = weblet.obj.inputs[i];
        weblet.frame.removeChild(weblet.obj.inputs[i]);
      }
      else
      {
        weblet.obj.inelements[i] = weblet.obj.inputs[i].parentNode;
        weblet.frame.removeChild(weblet.obj.inputs[i].parentNode);
      }
      weblet.obj.inelements[i].style.position = "absolute";
      weblet.obj.inelements[i].style.whiteSpace = "nowrap";
    }
    catch(e)
    {
      weblet.exception("personnal:time:day:" + i, e);
    }

  }

  weblet.showids = new Array();

  weblet.obj.tables.detail.clickCb["body"] = function(tab) { tab.weblet.dblclick_detail.call(tab.weblet, tab); }
  weblet.obj.tables.detail.setInvisible(0);
  weblet.obj.tables.detail.setInvisible(1);
  weblet.obj.tables.detail.setInvisible(2);
  weblet.obj.tables.detail.table.style.tableLayout = "fixed";

  weblet.obj.tables.time.clickCb["head"] = function(tab) { tab.weblet.sort_time.call(tab.weblet, tab); }
  weblet.obj.tables.time.clickCb["body"] = function(tab) { tab.weblet.click_time.call(tab.weblet, tab); }
  weblet.obj.tables.time.ignoreUnselect = true;
  weblet.obj.tables.time.setInvisible(0);
  weblet.obj.tables.time.setInvisible(1);
  weblet.obj.tables.time.setInvisible(2);

  weblet.obj.scols = weblet.initpar.timescols;

  weblet.obj.date = new Date();
  weblet.obj.date.setHours(0);
  weblet.obj.date.setMinutes(0);
  weblet.obj.date.setSeconds(0);
  
  weblet.obj.ajaxread = new MneAjaxData(weblet.win);

  weblet.obj.lasttime = 0;
  weblet.obj.slotstart = 0;

  var param =
  {
      "schema" : weblet.initpar.slotstartschema,
      "table"  : weblet.initpar.slotstarttable,
      "cols"   : "slotstart",
      "sqlend" : 1
  };

  weblet.obj.ajaxread.read("/db/utils/table/data.xml",  param)
  if ( weblet.obj.ajaxread.values.length > 0 )
    weblet.obj.lasttime = weblet.obj.slotstart = weblet.obj.ajaxread.values[0][0];

  weblet.getParam = MneAjaxWeblet.prototype.getParam;

  weblet.obj.rownum = -1;
  weblet.obj.colnum = -1;
  weblet.obj.ismodifyed = false;

  weblet.reset = function()
  {
    var i;
    for ( i in this.obj.inelements ) this.eleRemove(this.obj.inelements[i]);
    return MneAjaxWeblet.prototype.reset.call(this);
  }

  weblet.showValue = function(weblet)
  {
    var i;

    if ( typeof weblet != 'undefined' && weblet != null && typeof weblet.initpar.vdayname != 'undefined' )
    {
      var yy = parseInt(weblet.act_values[weblet.initpar.vdayname].substr(4,4));
      var mm = parseInt(weblet.act_values[weblet.initpar.vdayname].substr(2,2));
      var dd = parseInt(weblet.act_values[weblet.initpar.vdayname].substr(0,2));
      this.obj.date.setFullYear(yy);
      this.obj.date.setMonth(mm - 1);
      this.obj.date.setDate(dd);
      
        for ( i in this.obj.inputs )
          this.eleRemove(this.obj.inelements[i]);
        this.obj.rownum = -1;
        this.obj.colnum = -1;
  
    }

    this.obj.ismodifyed = false;
    if ( this.win.mne_config.username == 'admindb' && this.frame.login.value != '') this.obj.loginname = this.frame.login.value; else this.obj.loginname = 'session_user';

    this.obj.date.setHours(0);
    this.obj.date.setMinutes(0)
    this.obj.date.setSeconds(0);
    this.obj.daystart = parseInt(Number(this.obj.date.getTime()) / 1000);
    this.obj.fulldaynamestart = this.txtAddNull(this.obj.date.getDate(),2) + this.txtAddNull(this.obj.date.getMonth() + 1,2) + this.txtAddNull(this.obj.date.getFullYear(),4)

    this.act_values['vday'] = this.obj.date.getDate();
    this.act_values['vmonth'] = this.obj.date.getMonth() + 1;
    this.act_values['vyear'] = this.obj.date.getFullYear();
    
    this.obj.date.setDate(1);
    this.obj.month = parseInt(Number(this.obj.date.getTime()) / 1000);

    this.obj.date.setMonth(0);
    this.obj.year = parseInt(Number(this.obj.date.getTime()) / 1000);

    this.obj.date.setTime((this.obj.daystart + 86400 + 3600) * 1000);
    this.obj.date.setHours(0);
    this.obj.date.setMinutes(0)
    this.obj.date.setSeconds(0);
    this.obj.dayend = parseInt(Number(this.obj.date.getTime()) / 1000);

    this.obj.date.setTime(this.obj.daystart * 1000);

    this.act_values['loginname'] = this.obj.loginname;
    this.act_values['day'] = this.obj.daystart;

    this.initpar.tablecolstyle = this.initpar.personcolstyle;
    this.initpar.tablerowstyle = this.initpar.personrowstyle;
    this.initpar.tablerowstylecol = this.initpar.personrowstylecol;

    this.initpar.sschema = this.initpar.personschema;
    this.initpar.squery = this.initpar.personquery;
    this.initpar.cols = this.initpar.personcols;
    this.initpar.scols = 'sortcol';
    var p = {};

    p.maintable = 'detail';
    p.wcol = 'loginname,day,vfullday,loginname';
    p.wop = '=,=,o=,=',
    p.wval = this.obj.loginname + "," + this.obj.daystart + "," + this.obj.fulldaynamestart + "," + this.obj.loginname;

    MneAjaxWeblet.prototype.showTable.call(this, this, p);
    if ( this.values.length > 1 )
      this.obj.tables.detail.delRow(1);

    if ( this.obj.tables.detail.body.origdata.length > 0 )
    {
      for ( i in this.ids)
        this.act_values[i] = this.obj.tables.detail.body.origdata[0][this.ids[i]];

      this.act_values['day'] = this.obj.daystart;
      this.obj.personid = this.act_values.personid;

      this.show_time();
    }
    else
    {
      this.obj.personid = '';
      this.show_time();
    }

    this.act_values['loginname'] = this.obj.loginname;
    this.act_values['day'] = this.obj.daystart;
    this.act_values['month'] = this.obj.month;
    this.act_values['year'] = this.obj.year;
    this.act_values['personid'] = this.obj.personid;
    
    this.act_values['vday'] = this.obj.date.getDate();
    this.act_values['vmonth'] = this.obj.date.getMonth() + 1;
    this.act_values['vyear'] = this.obj.date.getFullYear();
      

  }    


  weblet.ok = function(t)
  {
    if ( this.write() == 'ok' )
    {
      this.obj.ismodifyed = false;
      this.obj.rownum = this.obj.tables.time.act_rownum;
      this.obj.colnum = this.obj.tables.time.tab2col(this.obj.tables.time.act_colnum);
      this.act_values.timeid = this.act_values.result;
      this.showValue(this);
      this.setDepends("showValue");
      return false;
    }

    this.click_time(this.obj.tables.time);
    return true;
  }

  weblet.add = function()
  {
    if ( this.obj.personid == '' ) return false;
    
    var r = this.obj.tables.time.add("");
    this.obj.tables.time.setData(r, this.obj.inputs.timeid.colnum, "################");
    this.obj.tables.time.setData(r, this.obj.inputs.clocktime.colnum, this.obj.lasttime, 'interval');
    if ( typeof this.obj.inputs.duration != 'undefined' )
      this.obj.tables.time.setData(r, this.obj.inputs.duration.colnum, 0, 'interval');
    else
      this.obj.tables.time.setData(r, this.obj.inputs.clocktimeend.colnum, this.obj.lasttime, 'interval');
  
    return false;
  }
  

  weblet.cancel = function()
  {
    this.obj.rownum = -1;
    this.obj.colnum = -1;
    this.obj.ismodifyed = false;
    this.showValue();
    return false;
  }

  weblet.del = function()
  {
    var str;

    if ( this.obj.inputs.timeid.value == '' )
    {
      this.error("#mne_lang#Bitte eine Zeile auswählen");
      return '';
    }

    var p =
    {
        schema : this.initpar.delschema,
        name : this.initpar.delfunction,
        typ0 : "text",
        sqlend : 1   
    }
    p = this.addParam(p, "par0", this.obj.inputs.timeid);

    if ( typeof this.obj.inputs.duration != 'undefined' )
      str = this.txtSprintf(this.txtGetText("#mne_lang#Eintrag <$1> wirklich löschen"), this.obj.inputs.clocktime.value + " / " + this.obj.inputs.duration.value);
    else
      str = this.txtSprintf(this.txtGetText("#mne_lang#Eintrag <$1> wirklich löschen"), this.obj.inputs.clocktime.value + " - " + this.obj.inputs.clocktimeend.value);

    if ( this.confirm(str) != true )
      return false;

    if ( MneAjaxWeblet.prototype.write.call(this,'/db/utils/connect/func/execute.xml', p) == 'ok')
    {
      this.obj.rownum = -1;
      this.obj.colnum = -1;
      this.obj.ismodifyed = false;
      this.showValue(null);
      this.setDepends("del");
      return false;
    }
    return true;
  }

  weblet.prev = function()
  {
    this.obj.buttons.prev.blur();
    this.obj.date.setTime((this.obj.daystart - 86400 + 3600) * 1000);
    this.obj.colnum = 0;
    this.showValue();
    this.setDepends("prev");
    return false;
  }

  weblet.next = function()
  {    
    this.obj.buttons.next.blur();
    this.obj.date.setTime((this.obj.daystart + 86400 + 3600) * 1000);
    this.obj.colnum = 0;
    this.showValue();
    this.setDepends("next");

    return false;
  }

  weblet.show_time = function()
  {
    var i;

    this.initpar.tablecolstyle = ',,' + this.initpar.timecolstyle;
    this.initpar.tablerowstyle = this.initpar.timerowstyle;
    this.initpar.tablerowstylecol = "" + (parseInt(this.initpar.timerowstylecol) + 2) ;
    this.initpar.stylePath = this.initpar.timestylePath;
    
    this.initpar.sschema = this.initpar.timeschema;
    this.initpar.squery = this.initpar.timequery;
    this.initpar.cols = "timeid,orderproducttimeid," + this.initpar.timecols;
    this.initpar.scols = this.obj.scols;

    var p = {};

    p.maintable = 'time';
    p.wcol = 'start,start,loginname';
    p.wop = '>=,<,=',
    p.wval = this.obj.daystart + "," + this.obj.dayend + "," + this.obj.loginname;

    this.showTable(this, p);

    if ( this.values.length > 0 ) 
    {
      if ( typeof this.obj.inputs.duration != 'undefined')
        this.obj.lasttime = Number(this.values[this.values.length - 1][this.ids['clocktime']]) + Number(this.values[this.values.length - 1][this.ids['duration']]);
      else
        this.obj.lasttime = Number(this.values[this.values.length - 1][this.ids['clocktimeend']]);
    }
    else
    {
      this.obj.lasttime = this.obj.slotstart;
    }

    this.obj.timeids = this.initpar.cols.split(',');
    this.obj.params = new Array();

    for ( i =0; i<this.obj.timeids.length; i++ )
    {
      this.obj.params[i] = {};
      this.obj.params[i]['typ'] = this.typs[i];
      this.obj.params[i]['name'] = this.names[i];
      this.obj.params[i]['id'] = this.obj.timeids[i];
    }

    for ( i in this.obj.inputs )
    {
      var ele = this.obj.inputs[i];
      if ( typeof this.regexps[this.ids[i]] != 'undefined' )
        this.create_checkpopup(ele, this.regexps[this.ids[i]]);

      if ( typeof ele.inputtyp == 'undefined' ) ele.inputtyp = this.inGetTyp(this.typs[this.ids[i]]);
      if ( ele.mykeyup != true ) ele.onkeyup = null;
      this.inClear(this.obj.inputs[i]);
      ele.rownum = -1;

      this.eleRemove(this.obj.inelements[i]);
    }

    for ( i in this.obj.inputs )
    {
      this.showInput(this.obj.inputs[i], '', this.obj.params[ele.colnum]['typ'], false);
      if ( typeof this.obj.inputs[i].inputcheckobject == 'string' || typeof this.obj.inputs[i].inputcheckobject == 'object')
      {
        if ( typeof this.obj.inputs[i].inputcheckobject == 'string' )
          this.obj.inputs[i].inputcheckobject = this.obj.inputs[this.obj.inputs[i].inputcheckobject];
        this.obj.inputs[i].inputcheckobject.onkeyup = null;
      }
      this.obj.inputs[i].setModify('modifyno');
    }

    while ( this.obj.personid != '' && this.obj.tables.time.body.rows.length < 12 )
      this.add();

    if ( this.obj.rownum != -1 && this.obj.colnum != -1 )
      this.obj.tables.time.findCell(this.obj.tables.time.body.rows[this.obj.rownum].cells[this.obj.tables.time.col2tab(this.obj.colnum)], false, false);

    return false;
  }


  weblet.write = function()
  {

    if ( this.obj.inputs.timeid.value == '' )
    {
      this.error("#mne_lang#Bitte eine Zeile auswählen");
      return '';
    }

    var value = this.obj.daystart + parseInt(this.obj.inputs.clocktime.mne_timevalue);
    if ( value == "NaN" || this.obj.inputs.clocktime.getModify() == 'modifywrong' )
    {
      this.error("#mne_lang#Bitte die Zeiteingaben überprüfen");
      return '';
    }

    var duration;
    if ( typeof this.obj.inputs.duration != 'undefined' )
    {
      duration = parseInt(this.obj.inputs.duration.mne_timevalue);
      if ( duration == "NaN" || this.obj.inputs.duration.getModify() == 'modifywrong' )
      {
        this.error("#mne_lang#Bitte die Zeiteingaben  überprüfen");
        return '';
      }
    }
    else
    {
      duration = this.obj.daystart + parseInt(this.obj.inputs.clocktimeend.mne_timevalue) - value;
      if ( duration == "NaN" || this.obj.inputs.clocktimeend.getModify() == 'modifywrong' )
      {
        this.error("#mne_lang#Bitte die Zeiteingaben überprüfen");
        return '';
      }
    }

    var p =
    {
        schema : this.initpar.okschema,
        name : this.initpar.okfunction,
        typ0 : "text",
        typ1 : "text",
        typ2 : "text",
        typ3 : "long",
        typ4 : "long",
        typ5 : "text",
        sqlend : 1   
    }
    p = this.addParam(p, "par0", this.obj.inputs.timeid);
    p = this.addParam(p, "par1", this.obj.inputs.orderproducttimeid);
    p = this.addParam(p, "par2", this.obj.personid);
    p = this.addParam(p, "par3", value);
    p = this.addParam(p, "par4", duration);
    p = this.addParam(p, "par5", this.obj.inputs.comment);

    retval = MneAjaxWeblet.prototype.write.call(this,'/db/utils/connect/func/execute.xml', p);
    if ( retval == 'ok' ) this.obj.modify = false;

    return retval;
  }

  weblet.dblclick_detail = function(tab)
  {
    tab.unselect();
    this.act_values.date = this.obj.daystart;

    this.parent.popups['selectday'].show();
    this.parent.popups['selectday'].weblet.onbtnobj = this;
    this.parent.popups['selectday'].weblet.showValue(this);
    {
      var popup = this.parent.popups['selectday'];
      var timeout = function() { popup.resize.call(popup, true, false ); }
      window.setTimeout(timeout, 100);
    }
    return false;

  }

  weblet.sort_time = function(tab)
  {
    var i;
    var col = tab.tab2col(tab.act_colnum);
    for ( i in this.ids ) if ( col == this.ids[i] ) break;

    this.obj.scols = i;
    this.show_time();
  }

  weblet.click_time = function(tab)
  {
    var i;
    var ele;
    var showdepend = false;
    try
    {
      if ( this.obj.rownum != -1 && this.obj.rownum != tab.act_rownum && this.obj.ismodifyed == true )
      {
        for ( i in this.obj.inputs ) { if ( typeof this.obj.inputs[i].onkeyup == 'function' ) this.obj.inputs[i].onkeyup(); }
        if ( this.ok('xxx') != false )
          this.obj.tables.time.findCell(this.obj.tables.time.body.rows[this.obj.rownum].cells[this.obj.tables.time.col2tab(this.obj.colnum)], false, false);
        this.obj.ismodifyed = false;
        return;
      }
    }
    catch( e )
    {
      this.exception("#mne_lang#Zeiteintrag:", e);
      this.obj.tables.time.findCell(this.obj.tables.time.body.rows[this.obj.rownum].cells[this.obj.tables.time.col2tab(this.obj.colnum)], false, false);
      return;
    }

    if ( this.inputlist != null )
    {
      this.inputlist.popup.hidden();
    }

    this.obj.rownum = tab.act_rownum;
    this.obj.colnum = tab.tab2col(tab.act_colnum);
    if ( this.obj.colnum == -1 ) return;

    try 
    {
      for ( i in this.obj.inputs )
      {
        ele = this.obj.inputs[i];
        if ( ele.rownum == tab.act_rownum )
        {
          var val = ( ele.inputtyp == 'date' || ele.inputtyp == 'time' || ele.inputtyp == 'interval' || ele.inputtyp == 'clocktime') ? ele.mne_timevalue : ele.value;
          if ( val != this.obj.tables.time.getData(this.obj.rownum, ele.colnum, true ))
          {
            this.obj.tables.time.setData(this.obj.rownum, ele.colnum, val, this.obj.params[ele.colnum]['typ']);
          }
        }
        else
        {
          showdepend = true;
          ele.setModify('modifyno');
          ele.rownum = tab.act_rownum;
        }

        this.eleRemove(this.obj.inelements[i]);
        this.act_values[i] = this.obj.tables.time.getData(tab.act_rownum, ele.colnum, true);
        this.showInput(ele, this.act_values[i], this.obj.params[ele.colnum]['typ'], this.obj.ismodifyed);
        var cell = this.obj.tables.time.body.rows[tab.act_rownum].cells[this.obj.tables.time.col2tab(this.obj.inputs[i].colnum)];
        this.eleMkClass(cell, this.obj.inputs[i].getModify(), true, "modify");
      }
    }
    catch(e)
    {
      this.exception('timeday:click_time',e)
    }


    var id = this.obj.timeids[this.obj.colnum];
    var param = this.obj.params[this.obj.colnum];

    ele = this.obj.inputs[id];
    if ( typeof ele == 'undefined' || ele.editable != true )
    {
      if ( showdepend ) this.setDepends('click');
      return;
    }

    var r = this.obj.tables.time.frame;
    ele = this.obj.inelements[id];
    ele.style.top = this.posGetTop(tab.act_cell, r) + "px";
    ele.style.left = this.posGetLeft(tab.act_cell, r) + "px";
    ele.style.width =  (this.posGetWidth(tab.act_cell)) + "px";
    ele.style.height = (this.posGetHeight(tab.act_cell)) + "px";
    ele.style.zIndex = 301;
    if ( typeof this.obj.inputs[id].onkeyup == 'function' ) this.obj.inputs[id].onkeyup();

    ele.param = param;
    this.obj.act_ele = ele;
    r.appendChild(ele);

    {
      var act_ele = this.obj.inputs[id];
      act_ele.style.width =  (this.posGetWidth(tab.act_cell)) + "px";
      act_ele.style.height = (this.posGetHeight(tab.act_cell)) + "px";
      act_ele.autopopup = ( act_ele.rdonly == true || typeof this.obj.fire != 'undefined' );
      var timeout = function()
      {
        if ( ( act_ele.getModify() == 'modifywrong' || act_ele.openallways == true || act_ele.autopopup == true ) && act_ele.autopopup == true  &&  act_ele.parentNode.className.indexOf('selectlist') != -1 )
        {
          MneDocEvents.prototype.fireEvent.call(act_ele.wrapper, act_ele.wrapper, 'click'); //act_ele.nextSibling.onclick();
        }
        else
        {
          act_ele.focus();
          act_ele.select();
        }
      }
      window.setTimeout(timeout, 0);
    }

    if ( showdepend ) this.setDepends('click');
  }

  weblet.find_empty = function()
  {
    var i;
    for ( i = 0; i<this.obj.tables.time.body.rows.length; i++)
    {
      if ( this.obj.tables.time.getData(i,0) == '################' )
      {
        this.obj.tables.time.findCell(this.obj.tables.time.body.rows[i].cells[0], false, false);
        return;
      }

    }
  }

  weblet.setModify = function(obj, modify)
  {
    if ( typeof obj != 'undefined' && obj != null && obj.id.indexOf("Input") >= 0 ) 
      this.obj.ismodifyed = ( modify != 'modifyno' );

    return MneAjaxWeblet.prototype.setModify.call(this, obj, modify);
  }

  weblet.onbtnclick = function(typ, button)
  {
    var i;

    if ( button.weblet.oid == 'selectday')
    {
      if ( typ != 'cancel' )
      {
        this.obj.date.setTime(button.weblet.act_values.date * 1000);
        this.showValue();
        this.setDepends("showValue");
      }
      return;
    }

    if ( typeof this.inputlist == 'undefined' || this.inputlist == null || typ != 'ok' ) return;

    if ( typeof this.inputlist.popup != 'undefined' && typeof this.inputlist.popup.weblet != 'undefined' )
      this.obj.fire = this.inputlist.popup.weblet.obj.fire;
    else
      this.obj.fire = i;

    if ( this.inputlist.element == 'ordernumber' )
    {
      this.obj.timeoutid_ordernumber = 'ordernumberbtnok';
      this.obj.timeout_ordernumber.call(this);
    }
    else if ( this.inputlist.element == 'productnumber')
    {
      this.obj.timeoutid_productnumber = 'productnumberbtnok';
      this.obj.timeout_productnumber.call(this);
    }

    var rownum = this.obj.inputs[this.inputlist.element].rownum;        
    for ( i  in this.obj.inputs )
    {
      var cell = this.obj.tables.time.setData(rownum,this.obj.inputs[i].colnum, this.obj.inputs[i].value);
      if ( cell != null )
      {
        this.eleMkClass(cell, this.obj.inputs[i].getModify(), true, "modify");
        if ( typeof this.obj.inputs[i].parentNode != 'undefined' && this.obj.inputs[i].parentNode != null )
        {
          var r = this.obj.tables.time.frame;
          ele = this.obj.inelements[i];
          ele.style.top = this.posGetTop(cell, r) + "px";
          ele.style.left = this.posGetLeft(cell, r) + "px";
          ele.style.width =  (this.posGetWidth(cell)) + "px";
          ele.style.height = (this.posGetHeight(cell)) + "px";
          ele = this.obj.inputs[i];
          ele.style.width =  (this.posGetWidth(cell)) + "px";
          ele.style.height = (this.posGetHeight(cell)) + "px";

          ele.focus();
        }
      }
    }
  }

  weblet.ordernumber_ready = function(w)
  {
    if ( w.values.length == 1 )
    {
      this.showInputExists(this.obj.inputs.orderid, w.values[0][1], null, true);
      this.showInputExists(this.obj.inputs.refname, w.values[0][2]);
      this.showInputExists(this.obj.inputs.orderdescription, w.values[0][3]);
      this.act_values.ordernumber = this.obj.inputs.ordernumber.value;
      this.setModify(this.obj.inputs.ordernumber,"modifyok");
      if ( weblet.win.mne_config.dayreportautotab.substr(0,1) == "t" ) this.ontabulator(this.obj.inputs.ordernumber);
    }
    else if ( w.values.length > 0 )
    {
      this.showInputExists(this.obj.inputs.orderid, "");
      this.showInputExists(this.obj.inputs.refname, "");
      this.showInputExists(this.obj.inputs.orderdescription, "");
      if ( this.obj.timeoutid_ordernumber == 'ordernumberbtnok' )
        this.setModify(this.obj.inputs.ordernumber,"modifyok");
      else
        this.setModify(this.obj.inputs.ordernumber,"modifywarning");
    }
    else
    {
      this.showInputExists(this.obj.inputs.orderid, "");
      this.showInputExists(this.obj.inputs.refname, "");
      this.showInputExists(this.obj.inputs.orderdescription, "");
      this.setModify(this.obj.inputs.ordernumber,"modifywrong");
    }

    this.showInputExists(this.obj.inputs.orderproducttimeid, "", null, true);
    this.showInputExists(this.obj.inputs.productnumber, "", null, true);
    this.showInputExists(this.obj.inputs.productname, "");
    this.showInputExists(this.obj.inputs.stepdescription, "");

    var rownum = this.obj.inputs['ordernumber'].rownum;        
    for ( i  in this.obj.inputs )
    {
      var cell = this.obj.tables.time.setData(rownum,this.obj.inputs[i].colnum, this.obj.inputs[i].value);
      if ( cell != null ) this.eleMkClass(cell, this.obj.inputs[i].getModify(), true, "modify");
    }
  }

  weblet.obj.timeout_ordernumber = function()
  {
    var w = new MneAjaxData(this.win);
    var param =
    {
        "schema" : this.initpar.orderschema,
        "query"  : this.initpar.orderquery,
        "cols"   : "ordernumber,orderid,refname,description",
        "closedInput.old" : "false",
        "openInput.old" : "true",
        "wcol" : 'ordernumber',
        "wop" : 'like',
        "wval" : this.obj.inputs.ordernumber.value + "%",
        "sqlend" : 1
    };

    if ( typeof this.obj.timeoutid_ordernumber == 'undefined' )
      return;

    delete this.obj.timeoutid_ordernumber;

    w.loadReadyWeblet = this;
    w.loadReady = function()
    {
      if ( this.req.readyState == 4 )
      {
        this.mk_result(this.check_status());
        this.loadReadyWeblet.ordernumber_ready.call(this.loadReadyWeblet, this);
      }
    }
    w.load("/db/utils/query/data.xml",  param, true);

  }

  weblet.productnumber2_ready = function(w)
  {
    if ( w.values.length == 0 )
    {
      this.showInputExists(this.obj.inputs.productname, "");
      this.showInputExists(this.obj.inputs.stepdescription, "");
      this.showInputExists(this.obj.inputs.orderproducttimeid, "", null, true);
      this.setModify(this.obj.inputs.productnumber,"modifywrong");
    }
    else
    {
      this.showInputExists(this.obj.inputs.productname, w.values[0][1]);
      this.showInputExists(this.obj.inputs.stepdescription, "");

      this.showInputExists(this.obj.inputs.orderproducttimeid, "", null, true);
      if ( w.values.length == 1 )
      {
        this.setModify(this.obj.inputs.productnumber,"modifyok");
        if ( weblet.win.mne_config.dayreportautotab.substr(0,1) == 't' ) this.ontabulator(this.obj.inputs.productnumber);
      }
      else
      {
        this.setModify(this.obj.inputs.productnumber,"modifywarning");
      }

    }

    var rownum = this.obj.inputs['productnumber'].rownum;        
    for ( i  in this.obj.inputs )
    {
      var cell = this.obj.tables.time.setData(rownum,this.obj.inputs[i].colnum, this.obj.inputs[i].value);
      if ( cell != null ) this.eleMkClass(cell, this.obj.inputs[i].getModify(), true, "modify");
    }
  }

  weblet.productnumber_ready = function(w)
  {
    if ( w.values.length > 0 )
    {
      this.showInputExists(this.obj.inputs.productname, w.values[0][0]);
      this.showInputExists(this.obj.inputs.stepdescription, "");

      this.showInputExists(this.obj.inputs.orderproducttimeid, "", null, true);
      this.setModify(this.obj.inputs.productnumber,"modifyok");
      if ( w.values.length == 1 )
      {
        this.showInputExists(this.obj.inputs.orderproducttimeid, w.values[0][1], null, true);
        this.showInputExists(this.obj.inputs.stepdescription, w.values[0][2]);
        if ( this.inputlist != null )
        {
          this.inputlist.popup.hidden();
        }
        if ( weblet.win.mne_config.dayreportautotab.substr(0,1) == 't' ) this.ontabulator(this.obj.inputs.stepdescription);
      }
      else
      {
        this.showInputExists(this.obj.inputs.orderproducttimeid, '', null, true);
        this.showInputExists(this.obj.inputs.stepdescription, '');
        if ( weblet.win.mne_config.dayreportautotab.substr(0,1) == 't' ) this.ontabulator(this.obj.inputs.productnumber);
      }
    }
    else
    {
      param =
      {
          "schema" : this.initpar.orderproducttimeschema,
          "query"  : this.initpar.orderproducttimecheckquery,
          "cols"   : "productnumber,productname",
          "orderidInput.old" : this.obj.inputs.orderid.value,
          "wcol" : 'productnumber',
          "wop" : 'like',
          "wval" : this.obj.inputs.productnumber.value + "%",
          "sqlend" : 1
      };

      w.loadReady = function()
      {
        if ( this.req.readyState == 4 )
        {
          this.mk_result(this.check_status());
          this.loadReadyWeblet.productnumber2_ready.call(this.loadReadyWeblet, this);
        }
      }
      w.load("/db/utils/query/data.xml",  param, true);
      return;
    }
    var rownum = this.obj.inputs['productnumber'].rownum;        
    for ( i  in this.obj.inputs )
    {
      cell = this.obj.tables.time.setData(rownum,this.obj.inputs[i].colnum, this.obj.inputs[i].value);
      if ( cell != null ) this.eleMkClass(cell, this.obj.inputs[i].getModify(), true, "modify");
    }
  }

  weblet.obj.timeout_productnumber = function()
  {
    var w = new MneAjaxData(this.win);
    var param =
    {
        "schema" : this.initpar.orderproducttimeschema,
        "query"  : this.initpar.orderproducttimequery,
        "cols"   : "productname,orderproducttimeid,stepdescription",
        "orderidInput.old" : this.obj.inputs.orderid.value,
        "productnumberInput.old" : this.obj.inputs.productnumber.value,
        "loginnameInput.old" : this.obj.loginname,
        "sqlend" : 1
    };

    if ( typeof this.obj.timeoutid_productnumber == 'undefined' )
      return;
    delete this.obj.timeoutid_productnumber;

    if ( this.obj.inputs.productnumber.value == '' )
      return;

    w.loadReadyWeblet = this;
    w.loadReady = function()
    {
      if ( this.req.readyState == 4 )
      {
        this.mk_result(this.check_status());
        this.loadReadyWeblet.productnumber_ready.call(this.loadReadyWeblet, this);
      }
    }
    w.load("/db/utils/query/data.xml",  param, true);
  }

  weblet.ontabulator = function(obj)
  {
    {
      var self = this;
      var next_colnum = obj.colnum + 1;
      while ( next_colnum < this.obj.editable.length && this.obj.editable[next_colnum].indexOf('tedit') == -1 ) next_colnum++;
      if ( next_colnum == this.obj.editable.length ) next_colnum = 2;
      var timeout = function() { try {  self.obj.tables.time.findCell(self.obj.tables.time.body.rows[self.obj.rownum].cells[self.obj.tables.time.col2tab(next_colnum)], false, false); } catch(e) { self.exception("ontabulator",e) }; };
      window.setTimeout(timeout, 0 );
    }
  }

  weblet.onbacktabulator = function(obj)
  {
    {
      var self = this;
      var next_colnum = obj.colnum - 1;
      if ( next_colnum == 2 ) next_colnum = this.obj.editable.length - 1;
      while ( next_colnum > 0 && this.obj.editable[next_colnum].indexOf('tedit') == -1 ) next_colnum--;
      var timeout = function() { try {  self.obj.tables.time.findCell(self.obj.tables.time.body.rows[self.obj.rownum].cells[self.obj.tables.time.col2tab(next_colnum)], false, false); } catch(e) { self.exception("ontabulator",e) }; };
      window.setTimeout(timeout, 0 );
    }
  }

  weblet.onkeyup = function(obj, e)
  {
    var evt = MneDocEvents.prototype.mkEvent(this.win, e)
    if ( evt.noevent == true || evt.keyCode == 9 )
      return;

    if ( evt.keyCode < 48 && evt.keyCode != 8 )
    {
      MneMisc.prototype.inOnmodify.call(obj, e);
      return;
    }

    if ( obj.id == 'ordernumberInput' )
    {
      var self = this;
      var timeout = function() { try { self.obj.timeout_ordernumber.call(self) } catch(e) {}; };
      if ( this.obj.timeoutid_ordernumber != 'undefined' )
        window.clearTimeout(this.obj.timeoutid_ordernumber);
      this.obj.timeoutid_ordernumber = window.setTimeout(timeout, this.initpar.returntimeout );
    }
    else if ( obj.id == 'productnumberInput' )
    {
      var self = this;
      var timeout = function() { try { self.obj.timeout_productnumber.call(self) } catch(e) {}; };
      if ( this.obj.timeoutid_productnumber != 'undefined' )
        window.clearTimeout(this.obj.timeoutid_productnumber);
      this.obj.timeoutid_productnumber = window.setTimeout(timeout, this.initpar.returntimeout );
    }
  }
}
