import cv2
img = cv2.imread("C:/Users/aleja/Pictures/wallpapers/New/4.jpg")

cv2.namedWindow("window", cv2.WND_PROP_FULLSCREEN)
cv2.setWindowProperty("window",cv2.WND_PROP_FULLSCREEN,cv2.cv.CV_WINDOW_FULLSCREEN)
cv2.imshow("window", img)

cv2.waitKey()
cv2.destroyAllWindows()