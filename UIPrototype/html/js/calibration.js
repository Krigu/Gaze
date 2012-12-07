var calibrationCircle = {
	
	imgPath: "img/circle_full.jpg", //url to image here
	imgMovingPath: "img/mario.gif", //url to moving image here
	imgWidth: 40, //standard width
	imgHeight: 40, //standard height
	animationTime: 2000, // time for the animation
		
	init : function(posX, posY){
		//create the image cloud
		var circle = document.createElement("IMG");
		$(circle).attr("src", calibrationCircle.imgPath);
		$(circle).attr("alt", "circle");
		$(circle).attr("class", "circle");
		$(circle).attr("id", "circleImg");

		$(document.body).append($(circle));

		calibrationCircle.display(posX, posY);		

	},
	
	display : function(posX, posY) {
		$("#circleImg").css("top", posY - (calibrationCircle.imgHeight / 2));
		$("#circleImg").css("left", posX - (calibrationCircle.imgWidth / 2));
		$("#circleImg").show(0);
	},

	move : function(posX, posY){
		$("#circleImg").attr("src", calibrationCircle.imgMovingPath);
		$("#circleImg").animate({ 
			top: posY - (calibrationCircle.imgHeight / 2),
			left: posX - (calibrationCircle.imgWidth / 2)
		}, calibrationCircle.animationTime, function() {
			$("#circleImg").attr("src", calibrationCircle.imgPath);
		 });
	
	},

	hide : function(){
		$("#circleImg").hide();
	}
}

$(document).ready(function() {
	   calibrationCircle.init();
});

