import pygame
from firebase import firebase
import os

images_path = '/home/pi/Documents/puliProject/COBO/images/'#'C:/Users/aleja/Documents/puliProject/COBO/images/'

_image_library = {}


pygame.init()
screen = pygame.display.set_mode((0, 0),pygame.FULLSCREEN)
done = False
clock = pygame.time.Clock()
pygame.mouse.set_cursor((8,8),(0,0),(0,0,0,0,0,0,0,0),(0,0,0,0,0,0,0,0)) #HIDE CURSOR
screen_width = 0.0
screen_heigth = 0.0
screen_width, screen_heigth = pygame.display.get_surface().get_size()
# print 'h',float(5/screen_width), 'w',float(8/screen_heigth)

image_width = int(screen_width/4)
image_heigth = screen_heigth/2
distace_between_images = int(screen_width/16)
distance_from_top = screen_heigth/4

firebase = firebase.FirebaseApplication('https://community-fbbae.firebaseio.com', None)

last_status = 0

i_sended_the_alert = False


def get_image(path):
        global _image_library
        image = _image_library.get(path)
        if image == None:
                canonicalized_path = path.replace('/', os.sep).replace('\\', os.sep)
                image = pygame.image.load(canonicalized_path)
                _image_library[path] = image
        return image

def icon_resize(image):
        return pygame.transform.scale(image, (image_width,image_heigth))

def screen_resize(image):
        return pygame.transform.scale(image, (screen_width,screen_heigth))

def show_main_screen():
        screen.blit(screen_resize(get_image(images_path + 'main_screen.png')),(0,0))

def show_crime_screen():
        if i_sended_the_alert:
                screen.blit(screen_resize(get_image(images_path + 'crime_cancel.png')),(0,0))
        else:
                screen.blit(screen_resize(get_image(images_path + 'crime.png')),(0,0))

def show_pe_screen():
        if i_sended_the_alert:
                screen.blit(screen_resize(get_image(images_path + 'pe_cancel.png')),(0,0))
        else:
                screen.blit(screen_resize(get_image(images_path + 'pe2.png')),(0,0))

def show_suspicious_screen():
        if i_sended_the_alert:
                screen.blit(screen_resize(get_image(images_path + 'suspicious_cancel.png')),(0,0))
        else:
                screen.blit(screen_resize(get_image(images_path + 'suspicious.png')),(0,0))


if __name__ == '__main__':             
        main_screen_is_showing = True
        show_main_screen()
        while not done:
                try:
                        alert_status = firebase.get('Board/Status',None)
                        
                        if alert_status != last_status: #Cheks if the status changed and if an alert is received it is displayed
                                main_screen_is_showing = False
                                if alert_status == 3:                  
                                        show_crime_screen()
                                elif alert_status == 2:
                                        show_pe_screen()
                                elif alert_status == 1:
                                        show_suspicious_screen()                        
                                else:                       
                                        show_main_screen()                    
                                        main_screen_is_showing = True
                                        i_sended_the_alert = False

                        for event in pygame.event.get():
                                if event.type == pygame.QUIT:
                                        done = True
                                        break
                                elif event.type == pygame.KEYDOWN:
                                        if event.key == pygame.K_ESCAPE:
                                                done = True
                                                break
                                elif event.type == pygame.MOUSEBUTTONUP:
                		        x,y = pygame.mouse.get_pos()
                                        # print x,y
                                        print x/float(screen_width) , y/float(screen_heigth)
                                        x = screen_width - x
                                        
                                        if main_screen_is_showing:
                                                if   0.359375 <= y/float(screen_heigth)  and  y/float(screen_heigth) <= 0.75625: #IS IN Y RANGE
                                                        if 0.0479166666667 <= x/float(screen_width) and x/float(screen_width) <= 0.304166666667:
                                                                screen.blit(screen_resize(get_image(images_path + 'pe_selected.png')),(0,0))
                                                                firebase.put('Board','Status',2)
                                                                i_sended_the_alert = True
                                                        elif 0.377083333333 <= x/float(screen_width) and x/float(screen_width) <= 0.635416666667:
                                                                screen.blit(screen_resize(get_image(images_path + 'suspicious_selected.png')),(0,0))
                                                                firebase.put('Board','Status',1)
                                                                i_sended_the_alert = True
                                                        elif 0.691666666667 <= x/float(screen_width) and x/float(screen_width) <= 0.947916666667:
                                                                screen.blit(screen_resize(get_image(images_path + 'crime_selected.png')),(0,0))
                                                                firebase.put('Board','Status',3)
                                                                i_sended_the_alert = True
                                        else:
                                                if ( 0.816666666667 <= x/float(screen_width) and x/float(screen_width) <= 0.966666666667) and (0.728125<= y/float(screen_heigth) and y/float(screen_heigth) <= 0.953125) and i_sended_the_alert:
                                                        if alert_status == 3:                  
                                                                screen.blit(screen_resize(get_image(images_path + 'c_cancelClick.png')),(0,0))
                                                        elif alert_status == 2:
                                                                screen.blit(screen_resize(get_image(images_path + 'pe_cancelClick.png')),(0,0))
                                                        elif alert_status == 1:
                                                                screen.blit(screen_resize(get_image(images_path + 's_cancelClick.png')),(0,0))   
                                                        firebase.put('Board','Status',0)
                                                 
                        last_status = alert_status

                except Exception as e:
                        print e
                        screen.blit(screen_resize(get_image(images_path + 'no_connection.png')),(0,0))
                        # screen.fill((255,255,255))

                pygame.display.flip()
                clock.tick(10)