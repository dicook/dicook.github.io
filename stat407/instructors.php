<?php

        $headers  = "From: Dianne Cook <dicook@iastate.edu>\r\n";
        $headers .= "Cc: dicook@iastate.edu\r\n";
        
        $subject = "Applied Multivariate Instructor Slides";
        $message = "Thank you for your interest in my Applied Multivariate Instructor slides. To access the slides, point your browser at:

http://streaming.stat.iastate.edu/~dicook/multivariatelectures/instructors/

And login using the use:
  Username: Instructor
  Password: multivar!
        
cheers,
Di
-------------
Dianne Cook
dicook@iastate.edu";




        $questions = array(
            array(
                'question' => "Which plot has three well-separated clusters?",
                'answer'   => 1,
                'choices'  => array(
	    			array("images/clusters.png","Flea beetles data."),
		    		array("images/clusters2.png","Olive oils data."),
			     	array("images/nonlinear.png","Music data."),
				    array("images/dependence-outliers.png","Chocolates data.")
                )
            ),
            array(
                'question' => "Which plot has one nonlinearly shaped cluster, and another group that has two sub-clusters?",
                'answer'   => 2,
                'choices'  => array(
		    		array("images/clusters.png","Flea beetles data."),
			    	array("images/clusters2.png","Olive oils data."),
			 	    array("images/nonlinear.png","Music data."),
    				array("images/dependence-outliers.png","Chocolates data.")
                )
            ),
            array(
                'question' => "Which plot has two groups separated along a nonlinear line of dependence?",
                'answer'   => 3,
                'choices'  => array(
				    array("images/clusters.png","Flea beetles data."),
    				array("images/clusters2.png","Olive oils data."),
	    		 	array("images/nonlinear.png","Music data."),
		    		array("images/dependence-outliers.png","Chocolates data.")
                )
            ),
            array(
                'question' => "Which plot has several outliers?",
                'answer'   => 4,
                'choices'  => array(
	    			array("images/clusters.png","Flea beetles data."),
		    		array("images/clusters2.png","Olive oils data."),
			     	array("images/nonlinear.png","Music data."),
				    array("images/dependence-outliers.png","Chocolates data.")
                )
            ) 
        );

    if(isset($_POST['q_num'])){

        if(isset($_POST['email']) && eregi("^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$", $_POST['email'])){
           if( isset($_POST['q_num']) && 
               isset($_POST['answer']) && $_POST['answer'] == $questions[$_POST['q_num']]['answer']) {
           

              $to = $_POST['email'];

              mail($to, $subject, $message, $headers);

              $html_message = "An e-mail has been sent to {$_POST['email']} with instructions for accessing this content.";

           }else {
                $html_message = "<b>OOPS!</b> That was the wrong answer. Try again.";
                $q_num = rand(0,count($questions)-1);
           }

        } else {
            $html_message = "<b>Please provide a valid e-mail address.</b>";
            $q_num = rand(0,count($questions)-1);
        }
    } else {
        $q_num = rand(0,count($questions)-1);
    }
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<!--
Design by Free CSS Templates
http://www.freecsstemplates.org
Released for free under a Creative Commons Attribution 2.5 License

Name       : Passageway  
Description: A two-column, fixed-width design with dark color scheme.
Version    : 1.0
Released   : 20100418

-->
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta name="keywords" content="" />
<meta name="description" content="" />
<meta http-equiv="content-type" content="text/html; charset=utf-8" />
<title>Lecture Notes for Applied Multivariate Statistics</title>
<link href="style.css" rel="stylesheet" type="text/css" media="screen" />
<script type="text/javascript" src="jquery/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="jquery/jquery.gallerax-0.2.js"></script>
</head>
<body>
<div id="wrapper">
	<div id="header">
		<div id="logo">
			<h1><a href="#">Applied Multivariate Statistics </a></h1>
			<p> <h2> Lecture Notes by <a href="http://www.public.iastate.edu/~dicook/">Di Cook</a></h2> </p>
		</div>
	</div>
	<div id="page">
		<div id="page-bgtop">
			<div id="page-bgbtm">
				<div id="content" style="padding-top: 125px">
                    <?php if(isset($html_message)){ echo "<p>$html_message</p>"; }?>
                    <?php if(isset($q_num)){ ?>
                    <span style="font-size: 18px; font-weight:bold;"><?php echo $questions[$q_num]['question'];?></span>
					<div>
						<div id="gallery">
							<div id="gallery-bgthumb">
								<ul class="thumbnails">
                                    <?php
                                        foreach($questions[$q_num]['choices'] as $q){
									        echo "<li><img src='{$q[0]}' title='{$q[1]}' style='width:243px; height:227px;' /></li>";
                                        }
                                    ?>
								</ul>
								<div style="clear:left;" ></div>
							</div>
						</div>
						<script type="text/javascript">

						/*$('#gallery').gallerax({
							outputSelector: 		'.output',					// Output selector
							thumbnailsSelector:		'.thumbnails li img',		// Thumbnails selector
							fade: 					'fast',						// Transition speed (fast)
							navNextSelector:		'.nav li a.navNext',		// 'Next' selector
							navPreviousSelector:	'.nav li a.navPrevious'		// 'Previous' selector
						});*/

					</script>
						<!-- end -->
					</div>
                    <div style="clear: left;"></div>
                    <br>
					<div class="post">
					    <form method="post" action="instructors.php" name="test">
                            <input type="hidden" name="q_num" value="<?php echo $q_num; ?>" />
                            <input type="radio" name="answer" value="1">Top-Left
                            <input type="radio" name="answer" value="2">Top-Right
                            <input type="radio" name="answer" value="3">Bottom-Left
                            <input type="radio" name="answer" value="4">Bottom-Right
                            <br><br>
                            <b>Also, please provide your e-mail address...</b>
                            <br>
                            <label for="email">Email Address:</label>
                            <input type="text" name="email"></input>
                            <br><br>
                            <input type="submit" value="Prove Thyself"></input>
                        </form>
                    </div>
                    
                    <?php } ?>
					
					<div style="clear: both;">&nbsp;</div>
				</div>
				<!-- end #content -->
				<div id="sidebar">
					<ul>
						<li>
							
							<div style="clear: both;">&nbsp;</div>
						</li>
						<li>
							<h2>For students</h2>
							<ul>
								<li><a href="students/intro-class.pdf">Introduction</a></li>
								<li><a href="students/graphics-class.pdf">Graphics</a></li>
								<li><a href="students/miss-class.pdf">Missing values</a></li>
								<li><a href="students/pca-class.pdf">Principal component analysis</a></li>
								<li><a href="students/discrim-class.pdf">Supervised classification</a></li>
								<li><a href="students/cluster-class.pdf">Cluster analysis</a></li>
								<li><a href="students/mvn-class.pdf">Multivariate normal distribution</a></li>
								<li><a href="students/factor-analysis-class.pdf">Explortory factor analysis</a></li>
								<li><a href="students/mresp-class.pdf">Inference for means</a></li>
								<li><a href="students/multreg-class.pdf">Multivariate regression</a></li>
								<li><a href="students/cancor-class.pdf">Canonical correlation analysis</a></li>
								<li><a href="students/corresp-class.pdf">Correspondence analysis</a></li>
								<li><a href="students/sem-class.pdf">Structural equation modeling</a></li>
						</ul>
						</li>
						<li>
							<h2>Movies</h2>
							<ul>
								<li><a href="students/graphics.mov">Graphics</a></li>
								<li><a href="students/miss.mov">Missing values</a></li>
								<li><a href="students/pca.mov">Principal component analysis</a></li>
								<li><a href="students/discrim.mov">Supervised classification</a></li>
								<li><a href="students/cluster.mov">Cluster analysis</a></li>
								<li><a href="students/mvn.mov">Multivariate normal distribution</a></li>
								<li><a href="students/factor-analysis.mov">Explortory factor analysis</a></li>
								<li><a href="students/mresp.mov">Inference for means</a></li>
						</ul>						
					      </li>
					      <li>
      							<h2>For instructors</h2>
					      </li>
					</ul>
				</div>
				<!-- end #sidebar -->
				<div style="clear: both;">&nbsp;</div>
			</div>
		</div>
	</div>
	<!-- end #page -->
</div>
<div id="footer">
	<p>Copyright (c) 2011 Dianne Cook. All rights reserved. Design adapted from Passageway by <a href="http://www.freecsstemplates.org/">Free CSS Templates</a>.</p>
</div>
<!-- end #footer -->
</body>
</html>
