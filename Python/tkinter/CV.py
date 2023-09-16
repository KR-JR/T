import cv2

img1 = cv2.imread('dog.jpg', cv2.IMREAD_COLOR)
img2 = cv2.stylization(img1, sigma_s = 100, sigma_r = 0.9)

cv2.imshow('origina', img1)
cv2.imshow('result', img2)
cv2.waitKey(0)
cv2.destroyAllWindows()

cv2.imwrite('result.jpg', img2)
