function Slideshow() {

    this.pos = 0;
    this.images = {};

    this.element = null;

    this.interval = null;
    this.duration = 1000;

    this.start  = function() {
        var obj = this;
        this.interval = setInterval(function(){obj.nextImage(obj);},this.duration);
    }
    this.stop = function() {
        clearInterval(this.interval);
    }

    this.nextImage = function(obj){
        obj.element.src = obj.images[obj.pos];
        
        
        obj.pos++;
        if(obj.pos > obj.images.length-1){
            obj.pos = 0;
        }
    }
}
