<?php
    $conn = mysqli_connect("mmydbpuli.clsxlmrwgdqn.us-east-2.rds.amazonaws.com", "puliroot", "puliroot2017", "CommunityPuli");
    
    if ($conn != null){
        $idUser=$_POST["id"];
        $fName = $_POST["fName"];
        $lName = $_POST["lName"];
        $username=$_POST["username"];
        $password=$_POST["passwrd"];
        $accessType=$_POST["accessType"];
        $street = $_POST["street"];
        $num = $_POST["num"];
        $neighborhood = $_POST["neighborhood"];
        $postalCode = $_POST["postalCode"];
        $city = $_POST["city"];
        $phone = $_POST["phone"];
    
        $sql = "INSERT INTO user (id, fName, lName,username, passwrd, accessType, street, num ,neighborhood , postalCode,  city, phone) 
            VALUES ('$idUser','$fName','$lName', '$username','$password','$accessType', '$street', '$num',  '$neighborhood',  
            '$postalCode','$city',$phone' )");
        
        // Run query and store resulting data
        $result = $conn->query($sql);

        if ($result == TRUE) {
            $conn -> close();
            return array("status" => "SUCCESS");   
        } 
        
        else{
            $conn -> close();
            return array ("status" => "Something went wrong");
        }
            
    }
        else {
            $conn -> close();
            header('HTTP/1.1 500 Bad connection, something went wrong while saving your data, please try again later');
     }
    
?>
