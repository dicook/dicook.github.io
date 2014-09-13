function particles(config) {
  return function() {    
    var width = Math.min(config.width, innerWidth),
    height = Math.min(config.height, innerHeight);
    
    var i = 0;
    
    var svg = d3.select("#content").insert("svg")
    .attr("width", width)
    .attr("height", height);
    
    svg.append("rect")
    .attr("width", width)
    .attr("height", height)
    .on("ontouchstart" in document ? "touchmove" : "mousemove", particle);
    
    function particle() {
      var m = d3.mouse(this);
      
      svg.insert("circle", "rect")
      .attr("cx", m[0])
      .attr("cy", m[1])
      .attr("r", 1e-6)
      .style("stroke", d3.hsl((i = (i + 1) % 360), 1, .5))
      .style("stroke-opacity", 1)
      .transition()
      .duration(2000)
      .ease(Math.sqrt)
      .attr("r", 100)
      .style("stroke-opacity", 1e-6)
      .remove();
      
      d3.event.preventDefault();
    }
  }
}