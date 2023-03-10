// ================================================================================
//
// Copyright: M.Nelson - technische Informatik
//            Die Software darf unter den Bedingungen 
//            der APGL ( Affero Gnu Public Licence ) genutzt werden
//            
// ================================================================================
{

  weblet.loadClass("MneAjaxSlider", "geometrie/mne_aslider.js");

  if ( weblet.doc.body.getAttribute("aria-view-mode") == 'mobile' )
  {
    var i;
    weblet.frame.appendChild( (weblet.root.container.top = weblet.doc.createElement('div')) );
    weblet.frame.appendChild( (weblet.root.container.bottom = weblet.doc.createElement('div')) );
    for ( i = 0; i < weblet.frame.childNodes.length; i++ ) weblet.frame.childNodes[i].className = 'container'
  }
  else
  {

    weblet.slider.fslider = weblet.slider.slider0 = new MneAjaxSlider(window, weblet.frame, "slider0", 'H', '50%', "move");

    weblet.root.container.top    = weblet.frame.sltop;
    weblet.root.container.bottom = weblet.frame.slbottom;
  }

}