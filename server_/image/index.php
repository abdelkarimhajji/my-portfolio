<?php 
    // header("Content-Length: 1000");
    // header("Location: /imgs/");
    // while(1);
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
<body>

    <h1><?php echo "hello word"   ?></h1>
  
    <h1><?php echo $_GET['name']  ?></h1>
    <h1><?php echo $_GET['email'] ?></h1>
    <h1><?php echo $_POST['name']  ?></h1>
    <h1><?php echo $_POST['email'] ?></h1>

    <?php
        if(isset($_FILES['file'])){
            echo "<h1>File name: " . $_FILES['file']['name'] . "</h1>";
            $fileContent = $_FILES['file']['tmp_name'];
          
            $uploadDirectory = "/Users/ahajji/Desktop/webserv/";

          
            $newFilePath = $uploadDirectory . basename($_FILES['file']['name']);

            if(move_uploaded_file($_FILES['file']['tmp_name'], $newFilePath)) {
                echo "The file has been uploaded successfully.";
            } else {
                echo "There was an error uploading the file.";
            }
        }   
    ?>
</body>
</html>