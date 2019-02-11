import pygame
from firebase import firebase
import os

images_path = 'C:/Users/aleja/Desktop/PULI/COBO/images/'

_image_library = {}


pygame.init()
screen = pygame.display.set_mode((0, 0),pygame.FULLSCREEN)
done = False
clock = pygame.time.Clock()
# pygame.mouse.set_cursor((8,8),(0,0),(0,0,0,0,0,0,0,0),(0,0,0,0,0,0,0,0)) #HIDE CURSOR

screen_width, screen_heigth = pygame.display.get_surface().get_size()

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
        screen.fill((0, 0, 0))
        screen.blit(icon_resize(get_image(images_path + 'crime.jpg')),(1*distace_between_images + 0*image_width, distance_from_top))
        screen.blit(icon_resize(get_image(images_path + 'PE.png')), (2*distace_between_images + 1*image_width, distance_from_top))
        screen.blit(icon_resize(get_image(images_path + 'suspicious.jpg')), (3*distace_between_images + 2*image_width, distance_from_top))

def show_crime_screen():
        screen.blit(screen_resize(get_image(images_path + 'crimen_screen.jpeg')),(0,0))
        if i_sended_the_alert:
                screen.blit(pygame.transform.scale(get_image(images_path + 'cancel.png'),(screen_width/2,screen_heigth/8)),(0, screen_heigth - screen_heigth/8))

def show_pe_screen():
        screen.blit(screen_resize(get_image(images_path + 'pe_screen.jpeg')),(0,0))
        if i_sended_the_alert:
                screen.blit(pygame.transform.scale(get_image(images_path + 'cancel.png'),(screen_width/2,screen_heigth/8)),(0, screen_heigth - screen_heigth/8))

def show_suspicious_screen():
        screen.blit(screen_resize(get_image(images_path + 'sospecha_screen.jpeg')),(0,0))
        if i_sended_the_alert:
                screen.blit(pygame.transform.scale(get_image(images_path + 'cancel.png'),(screen_width/2,screen_heigth/8)),(0, screen_heigth - screen_heigth/8))


if __name__ == '__main__':             
        main_screen_is_showing = True
        show_main_screen()
        while not done:
                alert_status = firebase.get('Board/Status',None)
                
                if alert_status != last_status: #Cheks if the status changed and if an alert is receibed it is displayed
                        main_screen_is_showing = False
                        if alert_status == 3:
                                show_suspicious_screen()
                        elif alert_status == 2:
                                show_pe_screen()
                        elif alert_status == 1:
                                show_crime_screen()                        
                        else:                       
                                show_main_screen()                    
                                main_screen_is_showing = True
                                i_sended_the_alert = False

                for event in pygame.event.get():
                        if event.type == pygame.QUIT:
                                done = True
                                break
                        elif event.type == pygame.MOUSEBUTTONUP:
        		        x,y = pygame.mouse.get_pos()
                                if main_screen_is_showing:
                                        if screen_heigth/4 <= y and y <= distance_from_top + image_heigth: #IS IN Y RANGE
                                                if distace_between_images <= x and x <= distace_between_images + image_width:
                                                        firebase.put('Board','Status',1)
                                                        i_sended_the_alert = True
                                                elif 2*distace_between_images + image_width<= x and x <= 2*distace_between_images + 2*image_width:
                                                        firebase.put('Board','Status',2)
                                                        i_sended_the_alert = True
                                                elif 3*distace_between_images + 2*image_width<= x and x <= 3*distace_between_images + 3*image_width:
                                                        firebase.put('Board','Status',3)
                                                        i_sended_the_alert = True
                                else:
                                        if (0 <= x and x <= screen_width/2) and (screen_heigth - screen_heigth/8 <= y and y <= screen_heigth) and i_sended_the_alert:
                                                firebase.put('Board','Status',0)
                
                pygame.display.flip()
                clock.tick(60)
                last_status = alert_status