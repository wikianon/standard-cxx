{
  var i,str;
  
  var ivalues = 
  {
      stylePath : '/weblet/builddiary/time/day',
      oid       : weblet.oid,

      timeschema      : 'mne_builddiary',
      timequery       : 'time',
      timecols        : "have_bd,presentid,btimeid,bdtimeid,orderid,ordernumber,productnumber,stepdescription,clocktime,clocktimeend,comment,refname,orderdescription,productname,bclocktime,bclocktimeend,weather,temperature",
      timeeditcols    : "ordernumber,productnumber,stepdescription,date,clocktime,duration,comment,bclocktime,bclocktimeend,weather,temperature",
      timecolstyle    : ",,,,,tedit,tedit,tedit rdonly,tedit,tedit,tedit,,,,tedit,tedit,tedit rdonly openallways,tedit",

      timerowstyle: 'bd',
      timerowstylecol : '0',
      
      okschema : 'mne_builddiary',
      delschema : 'mne_builddiary',
      
      styleName : 'style.css'
  };

  var svalues = 
  {
  };

  weblet.initDefaults(ivalues, svalues);

  weblet.loadbase('weblet/personnal/time/day', weblet, weblet.initpar.oid);
  weblet.obj.tables.time.setInvisible(3);
  weblet.obj.tables.time.setInvisible(4);
  weblet.obj.tables.time.setInvisible(5);
  weblet.obj.tables.time.setInvisible(6);
  
  weblet.write = function()
  {

    var value, bvalue, bduration, retval;
    
    if ( this.obj.inputs.timeid.value == '' )
    {
      this.error("#mne_lang#Bitte eine Zeile auswählen");
      return '';
    }

    value = this.obj.daystart + parseInt(this.obj.inputs.clocktime.mne_timevalue);
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

    if ( this.obj.inputs.bclocktime.value != '' && this.obj.inputs.bclocktimeend.value != '' )
    {
      bvalue = this.obj.daystart + parseInt(this.obj.inputs.bclocktime.mne_timevalue);
      if ( bvalue == "NaN" || this.obj.inputs.bclocktime.getModify() == 'modifywrong' )
      {
        this.error("#mne_lang#Bitte die Zeiteingaben Bautagebuch überprüfen");
        return '';
      }

      bduration = this.obj.daystart + parseInt(this.obj.inputs.bclocktimeend.mne_timevalue) - bvalue;
      if ( bduration == "NaN" || this.obj.inputs.bclocktimeend.getModify() == 'modifywrong' )
      {
        this.error("#mne_lang#Bitte die Zeiteingaben Bautagebuch überprüfen");
        return '';
      }

      if ( this.obj.inputs.weather.value == '' )
      {
        this.error("#mne_lang#Bitte das Wetter angeben");
        return '';

      }      
      if ( this.obj.inputs.temperature.value == '' )
      {
        this.error("#mne_lang#Bitte die Temperatur angeben");
        return '';

      }      
      
      if (( bduration < 0 || bvalue < value || bvalue > ( value + duration) || ( bvalue + bduration ) > ( value + duration ) ) )
      {
        this.warning("#mne_lang#Zeitangaben Bautagebuch und Zeiterfassung überschneiden sich");
      }      
      
    }
    else
    {
      bvalue = 0;
      bduration = 0;
    }

    var p =
    {
        schema : this.initpar.okschema,
        name : this.initpar.okfunction,

        typ0 : "text",
        typ1 : "text",
        typ2 : "text",
        typ3 : "text",
        typ4 : "long",
        typ5 : "long",
        typ6 : "text",

        typ7 : 'long',
        typ8 : 'long',
        typ9 : 'text',
        typ10 : 'long',

        sqlend : 1   
    }
    p = this.addParam(p, "par0", this.obj.inputs.presentid);
    p = this.addParam(p, "par1", this.obj.inputs.timeid);
    p = this.addParam(p, "par2", this.obj.inputs.orderproducttimeid);
    p = this.addParam(p, "par3", this.obj.personid);
    p = this.addParam(p, "par4", value);
    p = this.addParam(p, "par5", duration);
    p = this.addParam(p, "par6", this.obj.inputs.comment);

    p = this.addParam(p, "par7", bvalue);
    p = this.addParam(p, "par8", bduration);
    p = this.addParam(p, "par9", this.obj.inputs.weather);
    p = this.addParam(p, "par10", this.obj.inputs.temperature);

    retval = MneAjaxWeblet.prototype.write.call(this,'/db/utils/connect/func/execute.xml', p);
    if ( retval == 'ok' ) this.obj.modify = false;

    return retval;
  }

}
