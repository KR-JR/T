import pygame

pygame.init()
display = pygame.display.set_mode((800, 600))
myfont = pygame.font.SysFont('Comic Sans MS', 30)
score = 0

player = pygame.image.load("Python/SpaceShip(game)/images/spaceship.png")
playerX, playerY, playerDx , playerDy = 400, 550, 0, 0

alien=pygame.image.load("Python/SpaceShip(game)/images/alien.png")
alienX = [ ]     # 외계우주선의 x 위치 저장
alienY = [ ]     # 외계우주선의 y 위치 저장
alienDx = [ ]    # 외계우주선의 x 축 속도 저장 
alienDy = [ ]    # 외계우주선의 y 축 속도 저장 
alienNumber = 6   

for i in range(alienNumber):
    alienX.append(20+i*60)
    alienY.append(10)
    alienDx.append(0.1)
    alienDy.append(0.0)

missile = pygame.image.load('Python/SpaceShip(game)/images/missile.png')
missileX, missileY, missileDx , missileDy   = 0, 1000, 0, 0.1
missileState = "hidden"

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_LEFT:              playerDx = -0.1
            if event.key == pygame.K_RIGHT:             playerDx = 0.1
            if event.key == pygame.K_SPACE:
                if missileState == "hidden":
                    missileState = "fire"
                    missileX, missileY =playerX, playerY
        if event.type == pygame.KEYUP:
            if event.key == pygame.K_RIGHT or event.key == pygame.K_LEFT:
                playerDx = 0

    playerX += playerDx
    if playerX <= 0 : playerX = 0 
    if playerX > 750: playerX = 750

    display.fill((0, 0, 0))
    for i in range(alienNumber):         # 모든 외계우주선 움직임
        alienX[i] += alienDx[i]
        alienY[i] += alienDy[i]
        if  alienX[i] <= 0 or alienX[i] > 750:
            alienDx[i] *= -1
            alienY[i] += 30
        rect1 = pygame.Rect(alien.get_rect(topleft=(alienX[i], alienY[i])))
        rect2 = pygame.Rect(missile.get_rect(topleft=(missileX, missileY)))
        if rect1.colliderect(rect2) and missileState != "hidden":
            score += 1
            alienX[i], alienY[i], alienDx[i] , alienDy[i] = 0, 1000, 0.1, 0.0
        display.blit(alien, (alienX[i], alienY[i]))

    if missileY <= 0:
        missileY = 1000
        missileState = "hidden"

    if missileState == "fire":
        missileY -= missileDy

    display.blit(player, (playerX, playerY))
    display.blit(missile, (missileX, missileY))
    text = myfont.render(f'score={score}', False, (255, 255, 255))

    display.blit(text,(10,550))
    pygame.display.update()

pygame.quit()