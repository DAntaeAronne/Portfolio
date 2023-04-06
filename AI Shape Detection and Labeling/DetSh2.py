import numpy as np
import matplotlib.pyplot as plt
import cv2
import os
import re

# Turning a string into an integer if the string is made of numbers
def atoi(text):
    return int(text) if text.isdigit() else text

# Split the part of a string that is a number
# Turn that number into an integer
def natural_keys(text):
    return [ atoi(c) for c in re.split('(\d+)',text)]


# User puts in the name of one of the 9 types of patterns
# The corresponding folder is then opened to have the images be used
print(f"Image Folder(with \): ")
folder = input()

ImgPath = os.getcwd() + '\Images' + folder
ImgList  = os.listdir(ImgPath)
ImgList.sort(key=natural_keys)

# Going through each file
for dirName in ImgList:
    
    curImage = os.path.join(ImgPath,dirName)
    img = cv2.imread(curImage)

    # Grayscaling the image and blurring it to make it easier to read
    grayscale = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    blurred = cv2.GaussianBlur(grayscale, (3,3), 0)
    cv2.imshow('blurred', blurred)
    
    # Setting threshold of gray image
    _, threshold = cv2.threshold(blurred, 150, 255, cv2.THRESH_TOZERO_INV )
    cv2.imshow('threshold', threshold)
      
    # Using a findContours() function
    contours, _ = cv2.findContours(threshold, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    
    i = 0

    # List for storing names of shapes
    for contour in contours:
        # Here we are ignoring first counter because 
        # Findcontour function detects whole image as shape
        if i == 0:
            i = 1
            continue
        
        # cv2.approxPloyDP() function to approximate the shape
        approx = cv2.approxPolyDP(contour, 0.01 * cv2.arcLength(contour, True), True)
        
        # Using drawContours() function for debugging purposes
        cv2.drawContours(img, [contour], -1, (0, 0, 255), 2)
        
        # Finding center point of shape
        M = cv2.moments(contour)
        if M['m00'] != 0.0:
            x = int(M['m10']/M['m00'])
            y = int(M['m01']/M['m00'])

        # Putting shape name at center of each shape
        # For debugging purposes to make sure the shapes are being labeled correctly
        if len(approx) == 3:
            cv2.putText(img, 'Tri', (x, y), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 50, 0), 2)

        elif len(approx) == 4:
            cv2.putText(img, 'Quad', (x, y), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 50, 0), 2)

        elif len(approx) == 5:
            cv2.putText(img, 'Penta', (x, y), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 50, 0), 2)

        elif len(approx) == 6:
            cv2.putText(img, 'Hex', (x, y), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 50, 0), 2)

        else:
            cv2.putText(img, 'Circle', (x, y), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 50, 0), 2)


        # Displaying the image after drawing contours
    cv2.imshow('shapes', img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()


