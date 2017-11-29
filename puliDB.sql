
CREATE TABLE wearable ( 
    id INT AUTO_INCREMENT NOT NULL PRIMARY KEY, 
    rfidValue VARCHAR(70) NOT NULL,
); 

CREATE TABLE board (
    id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, 
); 

CREATE TABLE users (
    id INT AUTO_INCREMENT NOT NULL PRIMARY KEY, 
    fName VARCHAR(50) NOT NULL,
    lName VARCHAR(50) NOT NULL,
    street VARCHAR(30) NOT NULL,
    num VARCHAR(30) NOT NULL,
    neighborhood VARCHAR(30) NOT NULL,
    postalCode VARCHAR(30) NOT NULL,
    city VARCHAR(30) NOT NULL,
    phone VARCHAR(30) NOT NULL,
    boardID INT NOT NULL,
    wearableID INT NOT NULL,

    FOREIGN KEY (wearableID) REFERENCES wearable(id),
    FOREIGN KEY (boardId) REFERENCES board(id)
);


CREATE TABLE contacts (  
    id INT AUTO_INCREMENT NOT NULL PRIMARY KEY,
    userID INT NOT NULL,
    fName VARCHAR(50) NOT NULL,
    lName VARCHAR(50) NOT NULL,
    street VARCHAR(30) NOT NULL,
    num VARCHAR(30) NOT NULL,
    neighborhood VARCHAR(30) NOT NULL,
    postalCode VARCHAR(30) NOT NULL,
    city VARCHAR(30) NOT NULL,
    phone VARCHAR(30) NOT NULL,


    FOREIGN KEY (userID) REFERENCES users(id)

);

 CREATE TABLE recordEvents (  
    id INT AUTO_INCREMENT NOT NULL PRIMARY KEY,
    lat DECIMAL(12,9) NOT NULL,
    lon DECIMAL(12,9) NOT NULL,
    wearableID INT NOT NULL,
    boardID INT NOT NULL,
    dateEvent DATETIME NOT NULL, 

    FOREIGN KEY (wearableID) REFERENCES wearable(id),
    FOREIGN KEY (boardID) REFERENCES board(id)

);

 


