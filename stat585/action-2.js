function highlight(i) {
  var panel = document.getElementById(i);
  panel.setAttribute("fill", "#c5c5c5");
}

//                  <symbol id="gridSVG.pch16.high" viewBox="-5 -5 10 10" overflow="visible">
//                    <circle cx="0" cy="0" r="7.5"/>
//                  </symbol>


function radius(evt, i, factor) {
//  var x=evt.currentTarget;
//      console.log("The is of the triggered element (evt.target.id): " + evt.target.id);
//      console.log("The is of the triggered element (evt.currentTarget.id): " + x.id);
//      console.log("The is of the triggered element (ele.attributes[0].nodeName;):" + evt.target.attributes[0].nodeName);
//      console.log("The is of the triggered element (ele.attributes[0].nodeValue;):" + evt.target.attributes[0].nodeValue);
//      console.log("The is of the triggered element (evt.getAttribute('id')): " + evt.target.getAttribute("id"));
   
//  evt.target.setAttribute("width", evt.target.getAttribute("width")*factor);
//  evt.target.setAttribute("height", evt.target.getAttribute("height")*factor);
//  var shift=-evt.target.getAttribute("height")/2
//  evt.target.setAttribute("transform", 'translate('+shift+','+shift+')');
    // get the symbol:
    var symb = evt.target.getAttribute("xlink:href");
    document.getElementById(symb);
    
    evt.target.setAttribute("xlink:href", '#gridSVG.pch16.high');
}

function togglehigh(evt, i) {
  var panel = document.getElementById(i);
  if (panel.getAttribute("fill")=="#c5c5c5")
  	panel.setAttribute("fill", "#e5e5e5");
  else panel.setAttribute("fill", "#c5c5c5")
  
  var x=evt.currentTarget;
  alert("id of currentTarget: "+x.id);
  
}


function dim(i) {
  var panel = document.getElementById(i);
  panel.style.fill ="#e5e5e5";
}


function frame(evt) {
//  var panel = document.getElementById(i);
//  panel.setAttribute("stroke-width", "5");

  var x=evt.target;
  x.setAttribute("stroke-width", "5")

//  panel.setAttribute("stroke-opacity", "1");
//  panel.setAttribute("stroke", "#336699");
}

function deframe(evt) {
//  var panel = document.getElementById(i);
//  panel.setAttribute("stroke-width", 1);
  var x=evt.target;
  x.setAttribute("stroke-width", "1")
}


function showTooltip(evt, label) {
  
  // Getting rid of any existing tooltips
  hideTooltip();

  var svgNS = "http://www.w3.org/2000/svg";

  var target = evt.currentTarget;
  
  // Create new text node, rect and text for the tooltip
  var content = document.createTextNode(label);  

  var text = document.createElementNS(svgNS, "text");
  text.setAttribute("id", "tooltipText");
  // Resetting some style attributes
  text.setAttribute("font-size", "16px");
  text.setAttribute("fill", "black");
  text.setAttribute("stroke-width", "0");
  text.appendChild(content);

  var rect = document.createElementNS(svgNS, "rect");
  rect.setAttribute("id", "tooltipRect");

  // Add rect and text to the bottom of the document.
  // This is because SVG has a rendering order.
  // We want the tooltip to be on top, therefore inserting last.
  var wrappingGroup = document.getElementsByTagName("g")[0];
  wrappingGroup.appendChild(rect);
  wrappingGroup.appendChild(text);
  // Width of overall image
  var width = wrappingGroup.parentNode.getBBox().width;

  // Transforming the mouse location to the SVG coordinate system
  // Snippet lifted from: http://tech.groups.yahoo.com/group/svg-developers/message/52701
  var m = target.getScreenCTM();
  var p = document.documentElement.createSVGPoint();
  p.x = evt.clientX;
  p.y = evt.clientY;
  p = p.matrixTransform(m.inverse());

  // Determine position for tooltip based on location of 
  // element that mouse is over
  // AND size of text label
  // Currently the tooltip is offset by (3, 3)
  var tooltipx = p.x + 3;
  var tooltiplabx = tooltipx + 5;
  var tooltipy = p.y + 3;
  var tooltiplaby = tooltipy + 5;

  // Position tooltip rect and text
  text.setAttribute("transform", 
                    "translate(" + tooltiplabx + ", " + tooltiplaby + ") " +
                    "scale(1, -1)");

  rect.setAttribute("x", tooltipx);
  rect.setAttribute("y", tooltipy);
  rect.setAttribute("width", text.getBBox().width + 10);
  rect.setAttribute("height", text.getBBox().height + 5);
  rect.setAttribute("stroke", "black");
  rect.setAttribute("fill", "yellow");

  // If we are too close to the right-hand edge of the image
  // reposition rect and text to be top-left and right-aligned
  if (tooltipx + rect.getBBox().width > width) {
    tooltipx = p.x - text.getBBox().width - 10 - 3;
    tooltiplabx = tooltipx + 5;
    text.setAttribute("transform", 
                      "translate(" + tooltiplabx + ", " + tooltiplaby + ") " +
                      "scale(1, -1)");

    rect.setAttribute("x", tooltipx);
  } 
}

function hideTooltip() {
  // Remove tooltip text and rect
  var text = document.getElementById("tooltipText");
  var rect = document.getElementById("tooltipRect");

  if (text !== null && rect !== null) {
    text.parentNode.removeChild(text);
    rect.parentNode.removeChild(rect);
  }
}

