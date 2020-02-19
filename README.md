# Puli Project

This repository contains the technical development for the project Puli 001 developed by Tec de Monterrey. 
This is the result of five years of research and development by different engeneering students, 
that is the reason this repository is divided by dates/stages each containing the source code for different prototypes.

## Perso
Perso is the main device, it is a personal, mobile device capable of emmiting emergency alerts, received other Perso's or other Cobo's.
The device was designed to be small, ergonomic, able to connect to wifi and give haptic feedback to the user.

The latest schematic a board design for the electronics can be found in the next link:

https://easyeda.com/alexhor22/perso-wifi

The main microcontroller for the project is a *ESP12F*.

The source code for this device is in the *Perso* folder.

## Cobo
Cobo is a device meant to be stationary somewhere inside the house, connected to the home's wifi network. Cobo receives an emergency signal and is displayed 
on the front screen of the device. Additionally, Cobo is able to send emergncy alerts as well as Perso. It is important to mention that Perso's and Cobo's can transmit and 
receive alerts from other Perso's and/or Cobo's that belong to the same community.

The cobo device is based on the *Raspberrypi 3* with a 3.5'' touch screen.

The main code for Cobo is developed in python using the *pygame* library. It can be found in the *Cobo* folder.

## puliApi
puliApi is a git submodule that can be found [here](https://github.com/alexhor22/puliAPI).

This submodule is an API developed in *python* with *Flask*.

The purpose for this API is to function as intermediary between the devices. The API manages the alerts, receives them and broadcast them as requested.
The API uses a non-sql database (mongoDB) for recording the current status for a community, if the status is either "Normal" or "Emergency".  
The API has endpoints for changing the status of a community and for reading it. This way the devices throuth http requests can read and write alerts.

The API is up and running on *Heroku* in the following URL:

http://puliapi.herokuapp.com/

If accesed througth the web browser a very simple menu can be seen for managing communities and manually change the status associated to the community.