/**
  Fichier qui contient toute la logique du jeu.

  @author   RBR
  @date     décembre 2020
 */
#include "gamecore.h"
#include "sprite.h"

#include <QDebug>
#include <QSettings>
#include <QPainter>
#include <QString>
#include <QColor>

#include "gamescene.h"
#include "gamecanvas.h"
#include "resources.h"
#include "utilities.h"
#include "blueball.h"
#include "bouncingspritehandler.h"
#include "playertickhandler.h"
#include "bouncingspritehandler.h"


const int SCENE_WIDTH = 1280;
const int CENTERING_POS_X_BALL_RESPAWN = 20;
const int CENTERING_POS_Y_BALL_RESPAWN = 40;
const int BRICK_SIZE = 15;
const int MIN_VALUE_WALL = 32;
const int MAX_VALUE_WALL = 1250;
const float BOUNCING_AREA_SIZE = 86.5;
const QPointF QPOINT_CENTER_TEXT_LIFE(250,400);
const QPointF QPOINT_CENTER_TEXT(400,300);
const QPointF QPOINT_CENTER_TEXT_WIN(200,-100);
const QPointF QPOINT_CENTER_UNDER_TEXT_WIN(200,700);
const QPointF QPOINT_TEXT_MENU_BUTTON_PLAY(650,500);
const QPointF QPOINT_TEXT_MENU_BUTTON_LEAVE(650,600);

Sprite* m_pPlayer;
//! Initialise le contrôleur de jeu.
//! \param pGameCanvas  GameCanvas pour lequel cet objet travaille.
//! \param pParent      Pointeur sur le parent (afin d'obtenir une destruction automatique de cet objet).
GameCore::GameCore(GameCanvas* pGameCanvas, QObject* pParent) : QObject(pParent) {

    // Mémorise l'accès au canvas (qui gère le tick et l'affichage d'une scène)
    m_pGameCanvas = pGameCanvas;

    // Créé la scène de base et indique au canvas qu'il faut l'afficher.
    m_pScene = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_WIDTH / GameFramework::screenRatio());
    pGameCanvas->setCurrentScene(m_pScene);
    m_pScene->setBackgroundColor(colorBackGround);

    // Création scène menu
    m_pSceneMenu = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_WIDTH / GameFramework::screenRatio());

    // Création des boutons jouer et quitter
    m_pButtonPlay = new Sprite(GameFramework::imagesPath() + "buttonPlay.png");
    m_pButtonPlay->setPos(QPOINT_TEXT_MENU_BUTTON_PLAY);

    m_pButtonLeave = new Sprite(GameFramework::imagesPath() + "buttonLeave.png");
    m_pButtonLeave->setPos(QPOINT_TEXT_MENU_BUTTON_LEAVE);

    // Ajout du background au menu
    Sprite* pSpriteBackGround = new Sprite(GameFramework::imagesPath() + "backgroundCB.jpg");
    m_pSceneMenu->addSpriteToScene(pSpriteBackGround);
    pSpriteBackGround->setPos(-350,-350);
    m_pSceneMenu->createText(QPOINT_CENTER_TEXT,"Menu du jeu",100);

    // Ajout des boutons à la scène menu
    m_pSceneMenu->addSpriteToScene(m_pButtonPlay);
    m_pSceneMenu->addSpriteToScene(m_pButtonLeave);

    // Création scène gagnante
    m_pSceneWin = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_WIDTH / GameFramework::screenRatio());
    m_pWinGame = new Sprite(GameFramework::imagesPath() + "youWin.jpg");
    m_pTrophy = new Sprite (GameFramework::imagesPath() + "trophy.png");
    m_pWinGame->setPos(100, 20);
    m_pTrophy->setPos(540,20);
    m_pSceneWin->addSpriteToScene(m_pWinGame);
    m_pSceneWin->addSpriteToScene(m_pTrophy);

    // Création scène perdante
    m_pSceneLoss = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_WIDTH / GameFramework::screenRatio());
    m_pLossGame = new Sprite(GameFramework::imagesPath() + "gameover2.jpg");
    m_pLossGame->setPos(0, -150);
    m_pSceneLoss->addSpriteToScene(m_pLossGame);

    // Trace un rectangle blanc tout autour des limites de la scène.
    // m_pSceneMenu->addRect(m_pSceneMenu->sceneRect(), QPen(Qt::blue));

    // Instancier et initialiser les sprite ici :

    // Création de la zone de rebond
    setupBouncingArea();

    // Ajout du sprite joueur (le pad, le rectangle)
    Sprite* pSprite = new Sprite(GameFramework::imagesPath() + "rectangle2.png");
    m_pScene->addSpriteToScene(pSprite);
    pSprite->setPos(m_pScene->width()/2.00, 650);
    m_pPlayer = pSprite;

    // Création des blocs (18x3)
    for (int j=0;j<3;j++) {

        for (int i=0;i<18;i++) {
            // Ajout d'un sprite d'un sprite cube (obstacle à casser) et lui attribut un "id"
            Sprite* pBlocSprite = new Sprite(GameFramework::imagesPath() + "wall.png");
            m_pScene->addSpriteToScene(pBlocSprite);
            pBlocSprite->setPos(50 + spaceLines, 80 + spaceColumns);
            spaceLines += 65;
            pBlocSprite->setData(0,"bloc-a-detruire");
            connect(pBlocSprite, &Sprite::destroyed, this, &GameCore::onSpriteDestroyed);
        }
        // Réinitialise les valeurs et ajoutes une marge de 65 pour l'espacement des blocs.
        spaceLines = 0;
        spaceColumns += 65;
    }


    // Animations du personnage (rectangle)
    pSprite->addAnimationFrame(GameFramework::imagesPath() + "rectangle2_vert.png");
    pSprite->startAnimation(1000);

    // Positionne la boule sur le rectangle et attend l'intéraction (espace) de l'utilisateur.
    isWaiting = true;

    // Démarre le tick pour que les animations qui en dépendent fonctionnent correctement.
    // Attention : il est important que l'enclenchement du tick soit fait vers la fin de cette fonction,
    // sinon le temps passé jusqu'au premier tick (ElapsedTime) peut être élevé et provoquer de gros
    // déplacements, surtout si le déboggueur est démarré.
    m_pGameCanvas->startTick();

}

//! Destructeur de GameCore : efface les scènes
GameCore::~GameCore() {
    delete m_pScene;
    m_pScene = nullptr;
}

//! Traite la pression d'une touche.
//! \param key Numéro de la touche (voir les constantes Qt)
//!
void GameCore::keyPressed(int key) {

    emit notifyKeyPressed(key);

    switch(key) {
    case Qt::Key_Left:
        if(m_pPlayer->left())
            m_pPlayer->setX(m_pPlayer->x() - 40); break;

    case Qt::Key_Right:
        if(m_pPlayer->right() < m_pScene->width() - 10)
            m_pPlayer->setX(m_pPlayer->x() + 40); break;

    case Qt::Key_Space:
        m_keySpacePressed = true;
        break;
    case Qt::Key_Escape:
        m_keyEscPressed = true;
        break;
    }
}

//! Traite le relâchement d'une touche.
//! \param key Numéro de la touche (voir les constantes Qt)
void GameCore::keyReleased(int key) {
    emit notifyKeyReleased(key);

    switch(key) {

    case Qt::Key_Space:
        m_keySpacePressed = false;
        break;
    }

}

//! Cadence.
//! \param elapsedTimeInMilliseconds  Temps écoulé depuis le dernier appel.
void GameCore::tick(long long elapsedTimeInMilliseconds) {
    //float distance = PLAYER_SPEED * elapsedTimeInMilliseconds / 1000.0F * m_PlayerDirection;
    // Test si la balle dépasse la valeur du mur du bas
    if (m_pTennisBall->y() >= 685) {
        isWaiting = true;
        m_pTennisBall->unregisterFromTick();
        playerLife--;
        isDead = true;
    }

    // Bloque la balle au centre du rectangle tant que l'utilisateur ne clique pas espace ou sur sa souris
    if (isWaiting) {

        m_pTennisBall->setPos(m_pPlayer->x() - CENTERING_POS_X_BALL_RESPAWN, m_pPlayer->y() - CENTERING_POS_Y_BALL_RESPAWN);
        static_cast<BouncingSpriteHandler*>(m_pTennisBall->tickHandler())->setSpriteVelocity(150,150);
        // Si l'utilisateur appuie sur Espace ou effectue un clic avec la souris
        // la balle continue sa trajectoire normalement
        if (m_keySpacePressed || onClick) {

            // Supprime le texte, réinitialise les valeurs et redémarre le tick
            delete textLifePlayer;
            textLifePlayer = nullptr;
            isWaiting = false;
            onClick = false;
            m_pTennisBall->registerForTick();
        }
    }


    // Affiche le nombre de vie restant du joueur.
    if (isDead) {
        if (playerLife >= 2) {
            textLifePlayer = m_pScene->createText(QPOINT_CENTER_TEXT_LIFE,
                                                  QString("Il vous reste %1 vies.").arg(playerLife), 100);
        } else if (playerLife == 1){
            textLifePlayer = m_pScene->createText(QPOINT_CENTER_TEXT_LIFE,
                                                  QString("Il vous reste %1 vie.").arg(playerLife), 110);
        }

        // Fin de partie pour le joueur, il a utilisé toutes ses vies.
        if (playerLife == 0) {
            m_pGameCanvas->setCurrentScene(m_pSceneLoss);
            m_pSceneLoss->createText(QPOINT_CENTER_TEXT_WIN,"Appuyez sur ESC pour retourner au menu",50, colorReturnMenu);
        }
    }

    isDead = false;

    // Affiche la scène si le joueur a gagné.
    if (counterBlock == 0 ) {
        m_pGameCanvas->setCurrentScene(m_pSceneWin);
        m_pSceneWin->createText(QPOINT_CENTER_TEXT_WIN,"BRAVO ! Vous avez réussi ", 75);
        m_pSceneWin->createText(QPOINT_CENTER_UNDER_TEXT_WIN,"Appuyez sur ESC pour retourner au menu",50, colorReturnMenu);
    }

    // Retour au menu si l'utilisateur presse la touche Esc.
    if (m_keyEscPressed) {
        m_pGameCanvas->setCurrentScene(m_pSceneMenu);
    }

    // Si le pad sort des limites, le repositionne à l'intérieur
    if (m_pPlayer->x() <= MIN_VALUE_WALL) {
        m_pPlayer->setX(MIN_VALUE_WALL);
    } else if (m_pPlayer->x() >= MAX_VALUE_WALL)
        m_pPlayer->setX(MAX_VALUE_WALL);

}

//! La souris a été déplacée.
//! Pour que cet événement soit pris en compte, la propriété MouseTracking de GameView
//! doit être enclenchée avec GameCanvas::startMouseTracking().
void GameCore::mouseMoved(QPointF newMousePosition) {
    emit notifyMouseMoved(newMousePosition);

    // Fais en sorte que le pad ne puisse pas sortir des zones de collisions
    if (newMousePosition.x() >= MIN_VALUE_WALL && newMousePosition.x() <= MAX_VALUE_WALL) {
        m_pPlayer->setX(newMousePosition.x());
        m_pPlayer->setOffset(-m_pPlayer->boundingRect().width()/2, -m_pPlayer->boundingRect().height()/2);
    }
}

//! Traite l'appui sur un bouton de la souris.
void GameCore::mouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons) {
    emit notifyMouseButtonPressed(mousePosition, buttons);
    //m_pScene->spriteAt(mousePosition);
    onClick = true;
}

//! Traite le relâchement d'un bouton de la souris.
void GameCore::mouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons) {
    emit notifyMouseButtonReleased(mousePosition, buttons);
}


//! Construit la zone de rebond, ainsi que la balle de tennis qui va s'y déplacer.
void GameCore::setupBouncingArea() {

    // Création des briques de délimitation de la zone et placement
    QPixmap smallBrick(GameFramework::imagesPath() + "wall.png");
    smallBrick = smallBrick.scaled(BRICK_SIZE,BRICK_SIZE);

    // Création d'une image faite d'une suite horizontale de briques
    QPixmap horizontalWall(BRICK_SIZE * BOUNCING_AREA_SIZE, BRICK_SIZE);
    QPainter painterHW(&horizontalWall);
    for (int col = 0; col < BOUNCING_AREA_SIZE; col++)
        painterHW.drawPixmap(col * BRICK_SIZE,0, smallBrick);

    // Création d'une image faite d'une suite verticale de briques
    QPixmap verticalWall(BRICK_SIZE, BRICK_SIZE * BOUNCING_AREA_SIZE - 38);
    QPainter painterVW(&verticalWall);
    for (int col = 0; col < BOUNCING_AREA_SIZE; col++)
        painterVW.drawPixmap(0, col * BRICK_SIZE, smallBrick);

    // Ajout de 3 sprites (utilisant les murs horizontaux et verticaux) pour délimiter
    // une zone de rebond.
    m_pScene->addSpriteToScene(new Sprite(horizontalWall), 0, 0);

    m_pScene->addSpriteToScene(new Sprite(verticalWall), -15, 0);
    m_pScene->addSpriteToScene(new Sprite(verticalWall), 1281, 0);

    m_pScene->addRect(m_pScene->sceneRect(), QPen(Qt::black));

    // Création de la balle de tennis qui rebondi
    m_pTennisBall = new Sprite(GameFramework::imagesPath() + "basket.png");
    m_pTennisBall->setTickHandler(new BouncingSpriteHandler);
    m_pScene->addSpriteToScene(m_pTennisBall);

    m_pTennisBall->registerForTick();

}

void GameCore::onSpriteDestroyed(QObject* pSprite) {
    counterBlock--;
}
