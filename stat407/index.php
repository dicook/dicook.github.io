<?php

   /*Array of Questions 
     Add your questions here
     Format:
       array(
           'img' => 'IMAGE PATH'    Probably something like images/cat.jpg
           'que' => 'QUESTION TEXT' What you want to ask. Leave blank if the text is in the graphic
           'ans' => 'ANSWER'        They have to get this exactly right, if we need to give some
       )                              fudge room, we can figure that out.
   */

   $QUESTIONS = array(
        array(
            'img' => 'images/cat.jpg',
            'que' => 'What type of animal is this?',
            'ans' => 'cat'
        ),
        array(
            'img' => 'images/queen.jpg',
            'que' => 'What is her first name?',
            'ans' => 'Elizabeth'
        )
   );


   //E-Mail Information
   //You will want to edit this information too.
   $FROM    =  "dicook@iastate.edu";
   $SUBJECT =  "Multivariate Instructor Materials";
   $MESSAGE =  "Dear Instructor,
The URL is: http://streaming.stat.iastate.edu/~dicook/multivariatelectures/instructors/
The Username is: Instructor
The Password is: multivar!";

?>


<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
    <head>
        <!-- Insert any scripts or links here -->
    </head>
    <body>
<?php
// This will send the e-mail and print a success/fail message
if(isset($_POST['email'])){
    if(!check_email_address($_POST['email'])){
        echo "<h3 class='error'>Sorry! Please enter a valid e-mail address!</h3>";
        echo "<h4 class=error'>Problems? Please contact dicook@iastate.edu</h4>";
    } else if($_POST['answer'] != $QUESTIONS[$_POST['idx']]['ans']){
        echo "<h3 class='error'>Sorry that was an incorrect response.</h3>";
        echo "<h4 class='error'>Try again!</h4>";
    } else {
        $fh = fopen("addresses.txt",'a');
        fwrite($fh, $_POST['email']."\n");
        fclose($fh);

        $to      = $_POST['email'];
        $headers = "From: $FROM \r\n".
                   "Reply-To: $FROM \r\n".
                   "X-Mailer: PHP/".phpversion();

        $suc = mail($to, $SUBJECT, $MESSAGE, $headers);

        if($suc){
            echo "<h3>That's right! An e-mail has been sent to ".$to."</h3>";
            echo "<h4>Thank you for your interest in this material.</h4>";
            exit;
        } else {
            echo "<h3 class='error'>You were correct! However, the e-mail failed to send!</h3>";
            echo "<h4>Please contact dicook@iastate.edu</h4>";
        }
    }
}
?>
        <!-- Insert Rest of HTML body here -->

        <form method="POST">
            <?php
            $idx = rand(0,count($QUESTIONS)-1)
            ?>
            <img src="<?php echo $QUESTIONS[$idx]['img']; ?>"><br>
            <input type="hidden" id="idx" name="idx" value="<?php echo $idx;?>">
            <label for="answer"><?php echo $QUESTIONS[$idx]['que']; ?></label>
            <input id="answer" name="answer" type="text"><br><br>

            <label for="email">E-mail:</label>
            <input id="email" name="email" type="text">
            <button type="submit">Get Username/Password</button>
        </form>

    </body>
</html>



<?php
//Some Helpful Functions!


//from www.linuxjournal.com/article/9585
function check_email_address($email) {
  // First, we check that there's one @ symbol, 
  // and that the lengths are right.
  if (!ereg("^[^@]{1,64}@[^@]{1,255}$", $email)) {
    // Email invalid because wrong number of characters 
    // in one section or wrong number of @ symbols.
    return false;
  }
  // Split it into sections to make life easier
  $email_array = explode("@", $email);
  $local_array = explode(".", $email_array[0]);
  for ($i = 0; $i < sizeof($local_array); $i++) {
    if
(!ereg("^(([A-Za-z0-9!#$%&'*+/=?^_`{|}~-][A-Za-z0-9!#$%&
.'*+/=?^_`{|}~\.-]{0,63})|(\"[^(\\|\")]{0,62}\"))$",
$local_array[$i])) {
      return false;
    }
  }
  // Check if domain is IP. If not, 
  // it should be valid domain name
  if (!ereg("^\[?[0-9\.]+\]?$", $email_array[1])) {
    $domain_array = explode(".", $email_array[1]);
    if (sizeof($domain_array) < 2) {
        return false; // Not enough parts to domain
    }
    for ($i = 0; $i < sizeof($domain_array); $i++) {
      if
(!ereg("^(([A-Za-z0-9][A-Za-z0-9-]{0,61}[A-Za-z0-9])|
.([A-Za-z0-9]+))$",
$domain_array[$i])) {
        return false;
      }
    }
  }
  return true;
}





?>
