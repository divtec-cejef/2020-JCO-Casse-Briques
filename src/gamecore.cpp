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
#include <QCoreApplication>

#include "gamescene.h"
#include "gamecanvas.h"
#include "resources.h"
#include "utilities.h"
#include "bouncingspritehandler.h"
#include "playertickhandler.h"
#include "bouncingspritehandler.h"


// Initialisation des constantes
const int SCENE_WIDTH = 1280;
const int CENTERING_POS_X_BALL_RESPAWN = 20;
const int CENTERING_POS_Y_BALL_RESPAWN = 40;
const int BRICK_SIZE = 15;
const int MIN_VALUE_WALL = 32;
const int MAX_VALUE_WALL = 1250;
const int SPACE_LIFE_PLAYER = 75;
const float BOUNCING_AREA_SIZE = 86.5;
const QPointF QPOINT_CENTER_TEXT_LIFE(250,-125);
const QPointF QPOINT_CENTER_TEXT(400,300);
const QPointF QPOINT_CENTER_TEXT_WIN(200,-100);
const QPointF QPOINT_CENTER_UNDER_TEXT_WIN(200,700);
const QPointF QPOINT_TEXT_MENU_BUTTON_RESUME(650,500);
const QPointF QPOINT_TEXT_MENU_BUTTON_RESTART(650,600);
const QPointF QPOINT_TEXT_MENU_BUTTON_LEAVE(650,700);

//! Initialise le contrôleur de jeu.
//! \param pGameCanvas  GameCanvas pour lequel cet objet travaille.
//! \param pParent      Pointeur sur le parent (afin d'obtenir une destruction automatique de cet objet).
GameCore::GameCore(GameCanvas* pGameCanvas, QObject* pParent) : QObject(pParent) {

    // Mémorise l'accès au canvas (qui gère le tick et l'affichage d'une scène)
    m_pGameCanvas = pGameCanvas;

    // Créé la scène de base et indique au canvas qu'il faut l'afficher.
    m_pSceneGame = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_WIDTH / GameFramework::screenRatio());
    pGameCanvas->setCurrentScene(m_pSceneGame);
    m_pSceneGame->setBackgroundColor(colorBackGround);

    // Création scène menu
    m_pSceneMenu = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_WIDTH / GameFramework::screenRatio());

    // Création des boutons jouer et quitter
    m_pButtonResume = new Sprite(GameFramework::imagesPath() + "buttonResume.png");
    m_pButtonResume->setPos(QPOINT_TEXT_MENU_BUTTON_RESUME);

    m_pButtonRestart = new Sprite(GameFramework::imagesPath() + "buttonRestart.png");
    m_pButtonRestart->setPos(QPOINT_TEXT_MENU_BUTTON_RESTART);

    m_pButtonLeave = new Sprite(GameFramework::imagesPath() + "buttonLeave.png");
    m_pButtonLeave->setPos(QPOINT_TEXT_MENU_BUTTON_LEAVE);

    // Ajout du background au menu
    Sprite* pSpriteBackGround = new Sprite(GameFramework::imagesPath() + "backgroundCB.jpg");
    m_pSceneMenu->addSpriteToScene(pSpriteBackGround);
    pSpriteBackGround->setPos(-350,-350);
    m_pSceneMenu->createText(QPOINT_CENTER_TEXT,"Menu du jeu",100);

    // Ajout des boutons à la scène menu
    m_pSceneMenu->addSpriteToScene(m_pButtonResume);
    m_pSceneMenu->addSpriteToScene(m_pButtonRestart);
    m_pSceneMenu->addSpriteToScene(m_pButtonLeave);

    // Création scène gagnante
    m_pSceneWin = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_WIDTH / GameFramework::screenRatio());
    m_pWinGame = new Sprite(GameFramework::imagesPath() + "youWin.jpg");
    m_pTrophy = new Sprite (GameFramework::imagesPath() + "trophy.png");
    m_pWinGame->setPos(100, 120);
    m_pTrophy->setPos(540,75);
    m_pSceneWin->addSpriteToScene(m_pWinGame);
    m_pSceneWin->addSpriteToScene(m_pTrophy);

    // Création scène perdante
    m_pSceneLoss = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_WIDTH / GameFramework::screenRatio());
    m_pLossGame = new Sprite(GameFramework::imagesPath() + "gameover.jpg");
    m_pLossGame->setPos(0, -80);
    m_pSceneLoss->addSpriteToScene(m_pLossGame);

    // Création de la zone de rebond
    setupBouncingArea();

    // Création des blocs à détruire
    createBlock();

    // Création du joueur (le rectangle)
    createPlayer();

    // Création des sprites coeurs vie du joueur.
    createLifePlayer();

    // Démarre le tick pour que les animations qui en dépendent fonctionnent correctement.
    // Attention : il est important que l'enclenchement du tick soit fait vers la fin de cette fonction,
    // sinon le temps passé jusqu'au premier tick (ElapsedTime) peut être élevé et provoquer de gros
    // déplacements, surtout si le déboggueur est démarré.
    m_pGameCanvas->startTick();

}

//! Destructeur de GameCore : efface les scènes
GameCore::~GameCore() {
    delete m_pSceneGame;
    m_pSceneGame = nullptr;
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
        if(m_pPlayer->right() < m_pSceneGame->width() - 10)
            m_pPlayer->setX(m_pPlayer->x() + 40); break;

    case Qt::Key_Space:
        m_keySpacePressed = true;
        break;
    case Qt::Key_Escape:
        // Retour au menu si l'utilisateur presse la touche Esc.
        m_pGameCanvas->setCurrentScene(m_pSceneMenu);
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

    if (m_playerLife == 0) {
        m_pButtonResume->setOpacity(0);
        if (m_isRestart) {
            m_pButtonResume->setOpacity(1);
            m_playerLife = 3;
        }
    }

    // Test si la balle dépasse la valeur du mur du bas.
    if (m_pBall->y() >= 685) {
        m_isWaiting = true;
        m_pBall->unregisterFromTick();
        m_playerLife--;
        m_isDead = true;
    }

    // Bloque la balle au centre du rectangle tant que l'utilisateur ne clique pas espace ou sur sa souris
    if (m_isWaiting) {

        m_pBall->setPos(m_pPlayer->x() - CENTERING_POS_X_BALL_RESPAWN, m_pPlayer->y() - CENTERING_POS_Y_BALL_RESPAWN);
        static_cast<BouncingSpriteHandler*>(m_pBall->tickHandler())->setSpriteVelocity(125,125);
        // Si l'utilisateur appuie sur Espace ou effectue un clic gauche avec la souris
        // la balle continue sa trajectoire normalement.
        if (m_keySpacePressed || m_onClick) {

            // Supprime le texte, réinitialise les valeurs et redémarre le tick
            delete textLifePlayer;
            textLifePlayer = nullptr;
            m_isWaiting = false;
            m_onClick = false;
            m_pBall->registerForTick();
        }
    }

    // Affiche le nombre de vie(s) restant du joueur.
    if (m_isDead) {

        switch(m_playerLife)
        {
        case 2:
            textLifePlayer = m_pSceneGame->createText(QPOINT_CENTER_TEXT_LIFE,
                                                      QString("Il vous reste %1 vies.").arg(m_playerLife), 100);
            m_pLifePlayer3->deleteLater();
            break;
        case 1:
            textLifePlayer = m_pSceneGame->createText(QPOINT_CENTER_TEXT_LIFE,
                                                      QString("Il vous reste %1 vie.").arg(m_playerLife), 110);
            m_pLifePlayer2->deleteLater();
            break;
        case 0:
            m_pGameCanvas->setCurrentScene(m_pSceneLoss);
            m_pSceneLoss->createText(QPOINT_CENTER_TEXT_WIN,"Appuyez sur ESC pour retourner au menu",50, colorReturnMenu);
            break;
        default:
            textLifePlayer = m_pSceneGame->createText(QPOINT_CENTER_TEXT_LIFE,
                                                      QString("Vous n'avez plus de vie.\n Retournez au menu\n pour recommencer !"), 100);
        }
    }

    m_isDead = false;

    // Affiche la scène si le joueur a gagné.
    if (m_counterBlock == 0 ) {
        m_pGameCanvas->setCurrentScene(m_pSceneWin);
        m_pSceneWin->createText(QPOINT_CENTER_TEXT_WIN,"BRAVO ! Vous avez réussi ", 75);
        m_pSceneWin->createText(QPOINT_CENTER_UNDER_TEXT_WIN,"Appuyez sur ESC pour retourner au menu",50, colorReturnMenu);
    }

    // Si le pad sort des limites, le repositionne à l'intérieur
    if (m_pPlayer->x() <= MIN_VALUE_WALL) {
        m_pPlayer->setX(MIN_VALUE_WALL);
    } else if (m_pPlayer->x() >= MAX_VALUE_WALL)
        m_pPlayer->setX(MAX_VALUE_WALL);

    // Évite que la balle parte sans que l'utilisateur ait effectué une interaction
    m_onClick = false;
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
    m_onClick = true;

    // Test si l'utilisateur fait un clique gauche et qu'il est sur le menu.
    if (buttons.testFlag(Qt::LeftButton) && m_pGameCanvas->currentScene() == m_pSceneMenu) {

        // Si oui, vérifie si il clique sur le bouton jouer .
        if (m_pButtonResume == m_pSceneMenu->spriteAt(mousePosition)) {
            m_pGameCanvas->setCurrentScene(m_pSceneGame);

        } else if(m_pButtonRestart == m_pSceneMenu->spriteAt(mousePosition)) {
            // Recréer la scène de jeu et réinitialise les valeurs.
            m_pButtonLeave->setOpacity(0.7);
            m_isRestart = true;
            m_pSceneGame = nullptr;
            m_pSceneGame = m_pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_WIDTH / GameFramework::screenRatio());
            m_pSceneGame->setBackgroundColor(colorBackGround);
            createBlock();
            createPlayer();
            createLifePlayer();
            setupBouncingArea();
            m_pGameCanvas->setCurrentScene(m_pSceneGame);
            // Sinon vérifie si il clique sur le bouton quitter.
        } else if (m_pButtonLeave == m_pSceneMenu->spriteAt(mousePosition)) {
            QCoreApplication::quit();
        }
    }
    m_pButtonResume->setOpacity(1);
    m_pButtonRestart->setOpacity(1);
}

//! Traite le relâchement d'un bouton de la souris.
void GameCore::mouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons) {
    emit notifyMouseButtonReleased(mousePosition, buttons);
}


//! Construit la zone de rebond, ainsi que la balle de basket qui va s'y déplacer.
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
    m_pSceneGame->addSpriteToScene(new Sprite(horizontalWall), 0, 0);

    m_pSceneGame->addSpriteToScene(new Sprite(verticalWall), -15, 0);
    m_pSceneGame->addSpriteToScene(new Sprite(verticalWall), 1281, 0);

    m_pSceneGame->addRect(m_pSceneGame->sceneRect(), QPen(Qt::black));

    // Création de la balle de tennis qui rebondit
    m_pBall = new Sprite(GameFramework::imagesPath() + "basket.png");
    m_pBall->setTickHandler(new BouncingSpriteHandler);
    m_pSceneGame->addSpriteToScene(m_pBall);

    m_pBall->registerForTick();

}

void GameCore::onSpriteDestroyed(QObject* pSprite) {
    m_counterBlock--;
}

// Créer les blocs à détruire
void GameCore::createBlock() {
    // Création des blocs (18x3)
    for (int j=0;j<3;j++) {

        for (int i=0;i<18;i++) {
            // Ajout d'un sprite d'un sprite cube (obstacle à casser) et lui attribut un "id"
            Sprite* pBlocSprite = new Sprite(GameFramework::imagesPath() + "wall.png");
            m_pSceneGame->addSpriteToScene(pBlocSprite);
            pBlocSprite->setPos(50 + m_spaceLines, 80 + m_spaceColumns);
            m_spaceLines += 65;
            pBlocSprite->setData(0,"bloc-a-detruire");
            connect(pBlocSprite, &Sprite::destroyed, this, &GameCore::onSpriteDestroyed);
        }
        // Réinitialise les valeurs et ajoutes une marge de 65 pour l'espacement des blocs.
        m_spaceLines = 0;
        m_spaceColumns += 65;
    }
}

// Créer le joueur et les animatons (changement de couleur)
void GameCore::createPlayer() {
    // Ajout du sprite joueur (le pad, le rectangle)
    Sprite* pSprite = new Sprite(GameFramework::imagesPath() + "rectangle2.png");
    m_pSceneGame->addSpriteToScene(pSprite);
    pSprite->setPos(m_pSceneGame->width()/2.00, 650);
    m_pPlayer = pSprite;

    // Animations du personnage (rectangle)
    pSprite->addAnimationFrame(GameFramework::imagesPath() + "rectangle2_vert.png");
    pSprite->startAnimation(1000);
}

// Créer les vies du joueurs et les affiches sur l'écran.
void GameCore::createLifePlayer() {

    // Création des sprites pour afficher les vies
    m_pLifePlayer1 = new Sprite(GameFramework::imagesPath() + "coeurViePlein.png");
    m_pLifePlayer2 = new Sprite(GameFramework::imagesPath() + "coeurViePlein.png");
    m_pLifePlayer3 = new Sprite(GameFramework::imagesPath() + "coeurViePlein.png");

    m_pSceneGame->addSpriteToScene(m_pLifePlayer1);
    m_pSceneGame->addSpriteToScene(m_pLifePlayer2);
    m_pSceneGame->addSpriteToScene(m_pLifePlayer3);

    m_pLifePlayer1->setPos(m_playerLifeDisplayX, m_playerLifeDisplayY);
    m_pLifePlayer2->setPos(m_playerLifeDisplayX + SPACE_LIFE_PLAYER, m_playerLifeDisplayY);
    m_pLifePlayer3->setPos(m_playerLifeDisplayX + (2 * SPACE_LIFE_PLAYER), m_playerLifeDisplayY);

}
