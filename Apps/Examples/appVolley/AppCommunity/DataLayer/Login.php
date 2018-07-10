<?php
    $conn = mysqli_connect("mmydbpuli.clsxlmrwgdqn.us-east-2.rds.amazonaws.com", "puliroot", "puliroot2017", "CommunityPuli");
    
    if ($conn != null){
        
        $sql = "SELECT id, fName, lName, passwrd, accessType FROM Users WHERE matricula = '$mat'";
    
        $result = $conn->query($sql);
        if ($result->num_rows > 0)
        {
            $row = $result -> fetch_assoc();
            $conn -> close();
        
            return array("idUser" => $row["matricula"], "fName" => $row["fName"], "lName" => $row["lName"],
                         "password" => $row["passwrd"], "accessType" => $row["accessType"], "status" => "SUCCESS");
        }
        else{
            $conn -> close();
            return array("status" => "USERNAME NOT FOUND");
        }
    }else{
        $conn -> close();
        return array("status" => "CONNECTION WITH DB WENT WRONG");
    }
?>