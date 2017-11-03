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
    boardId INT NOT NULL,
    wearableId INT NOT NULL,

    FOREIGN KEY (wearableId) REFERENCES wearable(id),
    FOREIGN KEY (boardId) REFERENCES board(id)
);

CREATE TABLE wearable ( 
    id INT AUTO_INCREMENT NOT NULL PRIMARY KEY, 
    userId INT NOT NULL,
    rfidValue VARCHAR(50) NOT NULL,

    FOREIGN KEY (userId) REFERENCES user(id)
); 

CREATE TABLE board (
    id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, 
    wearableId INT NOT NULL,
   
    FOREIGN KEY (wearableId) REFERENCES wearable(id)
); 


CREATE TABLE contacts (  
    id INT AUTO_INCREMENT NOT NULL PRIMARY KEY,
    userid INT NOT NULL,
    fName VARCHAR(50) NOT NULL,
    lName VARCHAR(50) NOT NULL,
    street VARCHAR(30) NOT NULL,
    num VARCHAR(30) NOT NULL,
    neighborhood VARCHAR(30) NOT NULL,
    postalCode VARCHAR(30) NOT NULL,
    city VARCHAR(30) NOT NULL,
    phone VARCHAR(30) NOT NULL,


    FOREIGN KEY (userid) REFERENCES users(id)

);

 CREATE TABLE recordEvents (  
    id INT AUTO_INCREMENT NOT NULL PRIMARY KEY,
    lat DECIMAL(12,9) NOT NULL,
    lon DECIMAL(12,9) NOT NULL,
    wearableId INT NOT NULL,
    boardId INT NOT NULL,
    timeReg TIME NOT NULL, 
    dateReg DATE NOT NULL,

    FOREIGN KEY (wearableId) REFERENCES wearable(id),
    FOREIGN KEY (boardId) REFERENCES board(id)

);

 


