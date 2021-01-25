/**
  Fichier qui contient toute la logique du jeu casse-briques.

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

// Initialisation des constantes.
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
const QPointF QPOINT_TEXT_MENU_BUTTON_RESUME(500,500);
const QPointF QPOINT_TEXT_MENU_BUTTON_RESUME_TEXT(575,500);
const QPointF QPOINT_TEXT_MENU_BUTTON_RESTART(500,600);
const QPointF QPOINT_TEXT_MENU_BUTTON_RESTART_TEXT(575,600);
const QPointF QPOINT_TEXT_MENU_BUTTON_LEAVE(500,700);
const QPointF QPOINT_TEXT_MENU_BUTTON_LEAVE_TEXT(575,700);


//! Initialise le contrôleur de jeu.
//! \brief GameCore::GameCore
//! \param pGameCanvas  GameCanvas pour lequel cet objet travaille.
//! \param pParent      Pointeur sur le parent (afin d'obtenir une destruction automatique de cet objet).
GameCore::GameCore(GameCanvas* pGameCanvas, QObject* pParent) : QObject(pParent) {

    // Mémorise l'accès au canvas (qui gère le tick et l'affichage d'une scène)
    m_pGameCanvas = pGameCanvas;

    // Création des différentes scènes du jeu.
    createSceneGame();
    createSceneMenu();
    createSceneWin();
    createSceneLoss();

    // Création de la zone de rebond.
    setupBouncingArea();

    // Création des blocs à détruire.
    createBlock();

    // Création du joueur (le rectangle).
    createPlayer();

    // Création des sprites coeurs vie du joueur.
    createLifePlayer();

    // Création du background pour le menu du jeu.
    setupBackGroundMenu();

    // Création des boutons pour le menu.
    createButton();

    // Définis la scène actuelle du jeu (le menu)
    m_pGameCanvas->setCurrentScene(m_pSceneMenu);

    // Démarre le tick pour que les animations qui en dépendent fonctionnent correctement.
    m_pGameCanvas->startTick();

}

//! Destructeur de GameCore : efface les scènes.
//! \brief GameCore::~GameCore
GameCore::~GameCore() {
    delete m_pSceneGame;
    m_pSceneGame = nullptr;
}

//! Traite la pression d'une touche.
//! \brief GameCore::keyPressed
//! \param key Numéro de la touche (voir les constantes Qt).
void GameCore::keyPressed(int key) {

    emit notifyKeyPressed(key);

    switch(key) {
    // Si le joueur presse la flèche de gauche
    case Qt::Key_Left:
        if(m_pPlayer->left()){
            m_pPlayer->setX(m_pPlayer->x() - 40);
        }
        break;

    // Si le joueur press la fléche de droite
    case Qt::Key_Right:
        if(m_pPlayer->right() < m_pSceneGame->width() - 10) {
            m_pPlayer->setX(m_pPlayer->x() + 40);
        }
        break;

    // Si le joueur presse Espace
    case Qt::Key_Space:
        m_keySpacePressed = true;
        break;
    // Si le joueur presse Echap
    case Qt::Key_Escape:
        // Retour au menu si l'utilisateur presse la touche Esc.
        m_pGameCanvas->setCurrentScene(m_pSceneMenu);
        m_textMenuResume->setText("Reprendre");
    }
}

//! Traite le relâchement d'une touche.
//! \brief GameCore::keyReleased
//! \param key Numéro de la touche (voir les constantes Qt).
void GameCore::keyReleased(int key) {
    emit notifyKeyReleased(key);

    switch(key) {

    case Qt::Key_Space:
        m_keySpacePressed = false;
        break;
    }

}

//! Gère la Cadence.
//! \brief GameCore::tick
//! \param elapsedTimeInMilliseconds  Temps écoulé depuis le dernier appel.
void GameCore::tick(long long elapsedTimeInMilliseconds) {
    //float distance = PLAYER_SPEED * elapsedTimeInMilliseconds / 1000.0F * m_PlayerDirection;

    // Enlève le bouton pause si l'utilisateur ne recommence pas la partie
    if (m_playerLife == 0) {
        m_pButtonResume->hide();
        m_textMenuResume->hide();
        if (m_isRestart) {
            m_pButtonResume->show();
            m_textMenuResume->show();
            m_playerLife = 3;
            m_isRestart = false;
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
        //static_cast<BouncingSpriteHandler*>(m_pBall->tickHandler())->setSpriteVelocity(125,125);
        // Si l'utilisateur appuie sur Espace ou effectue un clic gauche avec la souris
        // la balle continue sa trajectoire normalement.
        if (m_keySpacePressed || m_onClick) {

            // Supprime le texte, réinitialise les valeurs et redémarre le tick
            delete m_textLifePlayer;
            m_textLifePlayer = nullptr;
            m_isWaiting = false;
            m_onClick = false;
            m_pBall->registerForTick();
        }
    }


    // Vérifie si le joueur vient de perdre une vie.
    if (m_isDead) {

        // Affiche le nombre de vie(s) restant du joueur.
        switch(m_playerLife)
        {
        case 2:
            m_textLifePlayer = m_pSceneGame->createText(QPOINT_CENTER_TEXT_LIFE,
                                                        QString("Il vous reste %1 vies.").arg(m_playerLife), 100);
            m_pLifePlayer3->deleteLater();
            m_pButtonRestart->show();
            m_textMenuRestart->show();
            break;
        case 1:
            m_textLifePlayer = m_pSceneGame->createText(QPOINT_CENTER_TEXT_LIFE,
                                                        QString("Il vous reste %1 vie.").arg(m_playerLife), 110);
            m_pLifePlayer2->deleteLater();
            break;
        case 0:
            m_pGameCanvas->setCurrentScene(m_pSceneLoss);
            m_pSceneLoss->createText(QPOINT_CENTER_TEXT_WIN,"Appuyez sur ESC pour retourner au menu",50, m_colorReturnMenu);
            break;
        default:
            m_textLifePlayer = m_pSceneGame->createText(QPOINT_CENTER_TEXT_LIFE,
                                                        QString("Vous n'avez plus de vie.\n Retournez au menu\n pour recommencer !"), 100);
        }
    }

    m_isDead = false;

    // Affiche la scène si le joueur a gagné.
    if (m_counterBlock == 0) {
        m_pGameCanvas->setCurrentScene(m_pSceneWin);
        m_pSceneWin->createText(QPOINT_CENTER_TEXT_WIN,"BRAVO ! Vous avez réussi ", 75);
        m_pSceneWin->createText(QPOINT_CENTER_UNDER_TEXT_WIN,"Appuyez sur ESC pour retourner au menu",50, m_colorReturnMenu);
    }

    // Si le pad sort des limites, le repositionne à l'intérieur.
    if (m_pPlayer->x() <= MIN_VALUE_WALL) {
        m_pPlayer->setX(MIN_VALUE_WALL);
    } else if (m_pPlayer->x() >= MAX_VALUE_WALL)
        m_pPlayer->setX(MAX_VALUE_WALL);

    // Évite que la balle parte sans que l'utilisateur ait effectué une interaction.
    m_onClick = false;
}

//! Traite le déplacement de la souris.
//! \brief GameCore::mouseMoved
//! \param newMousePosition Nouvelle position de la souris.
void GameCore::mouseMoved(QPointF newMousePosition) {
    emit notifyMouseMoved(newMousePosition);

    // Fais en sorte que le pad ne puisse pas sortir des zones de collisions
    if (newMousePosition.x() >= MIN_VALUE_WALL && newMousePosition.x() <= MAX_VALUE_WALL) {
        m_pPlayer->setX(newMousePosition.x());
        m_pPlayer->setOffset(-m_pPlayer->boundingRect().width()/2, -m_pPlayer->boundingRect().height()/2);
    }
}

//! Traite l'appui sur un bouton de la souris.
//! \brief GameCore::mouseButtonPressed
//! \param mousePosition Position de la souris.
//! \param buttons Bouton de la souris.
void GameCore::mouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons) {
    emit notifyMouseButtonPressed(mousePosition, buttons);
    m_onClick = true;

    // Test si l'utilisateur fait un clique gauche et qu'il est sur le menu.
    if (buttons.testFlag(Qt::LeftButton) && m_pGameCanvas->currentScene() == m_pSceneMenu) {

        // Si oui, vérifie si il clique sur le bouton reprendre le jeu .
        if (m_pButtonResume == m_pSceneMenu->spriteAt(mousePosition)) {
            m_pGameCanvas->setCurrentScene(m_pSceneGame);

            // Sinon vérifie si il clique sur recommencer une partie
        } else if (m_pButtonRestart == m_pSceneMenu->spriteAt(mousePosition)) {
            // Recréer la scène de jeu et réinitialise les valeurs.
            m_pButtonRestart->setOpacity(0.7);
            m_isRestart = true;
            m_pSceneGame = nullptr;
            m_pSceneGame = m_pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_WIDTH / GameFramework::screenRatio());
            m_pSceneGame->setBackgroundColor(m_colorBackGround);

            // Réinitialise les valeurs pour que les blocs ne soient pas décalés et repositionne la boule sur le pad
            m_spaceColumns = 0;
            m_counterBlock = 54;
            m_isWaiting = true;

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
//! \brief GameCore::mouseButtonReleased
//! \param mousePosition Position de la souris.
//! \param buttons Bouton de la souris.
void GameCore::mouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons) {
    emit notifyMouseButtonReleased(mousePosition, buttons);
}


//! Construit la zone de rebond, ainsi que la balle qui va s'y déplacer.
//! \brief GameCore::setupBouncingArea
void GameCore::setupBouncingArea() {

    // Création des briques de délimitation de la zone et placement
    QPixmap smallBrick(GameFramework::imagesPath() + "wall.png");
    smallBrick = smallBrick.scaled(BRICK_SIZE,BRICK_SIZE);

    // Création d'une image faite d'une suite horizontale de briques.
    QPixmap horizontalWall(BRICK_SIZE * BOUNCING_AREA_SIZE, BRICK_SIZE);
    QPainter painterHW(&horizontalWall);
    for (int col = 0; col < BOUNCING_AREA_SIZE; col++)
        painterHW.drawPixmap(col * BRICK_SIZE,0, smallBrick);

    // Création d'une image faite d'une suite verticale de briques.
    QPixmap verticalWall(BRICK_SIZE, BRICK_SIZE * BOUNCING_AREA_SIZE - 38);
    QPainter painterVW(&verticalWall);
    for (int col = 0; col < BOUNCING_AREA_SIZE; col++)
        painterVW.drawPixmap(0, col * BRICK_SIZE, smallBrick);

    // Ajout de 3 sprites (utilisant les murs horizontaux et verticaux) pour délimiter.
    // une zone de rebond.
    m_pSceneGame->addSpriteToScene(new Sprite(horizontalWall), 0, 0);
    m_pSceneGame->addSpriteToScene(new Sprite(verticalWall), -15, 0);
    m_pSceneGame->addSpriteToScene(new Sprite(verticalWall), 1281, 0);

    m_pSceneGame->addRect(m_pSceneGame->sceneRect(), QPen(Qt::black));

    // Création de la balle de basketball qui rebondit.
    m_pBall = new Sprite(GameFramework::imagesPath() + "basket.png");
    m_pBall->setTickHandler(new BouncingSpriteHandler);
    m_pSceneGame->addSpriteToScene(m_pBall);

    m_pBall->registerForTick();

}

//! Déincrémente le compteur si un bloc est détruit.
//! \brief GameCore::onSpriteDestroyed
void GameCore::onSpriteDestroyed() {
    m_counterBlock--;
}

//! Créer les blocs à détruire.
//! \brief GameCore::createBlock
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

//! Créer le joueur et les animatons (changement de couleur).
//! \brief GameCore::createPlayer
void GameCore::createPlayer() {
    // Ajout du sprite joueur (le pad, le rectangle).
    Sprite* pSprite = new Sprite(GameFramework::imagesPath() + "rectangle2.png");
    m_pSceneGame->addSpriteToScene(pSprite);
    pSprite->setPos(m_pSceneGame->width()/2.00, 650);
    m_pPlayer = pSprite;

    // Animations du personnage (rectangle)
    pSprite->addAnimationFrame(GameFramework::imagesPath() + "rectangle2_vert.png");
    pSprite->startAnimation(1000);
}

//! Créer l'affichage des vies du joueur sur l'écran.
//! \brief GameCore::createLifePlayer
void GameCore::createLifePlayer() {

    // Création des sprites pour afficher les vies.
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

//! Créer les boutons d'interaction du menu du jeu.
//! \brief GameCore::createButton
void GameCore::createButton() {
    // Création des boutons pause recommencer et quitter.
    m_pButtonResume = new Sprite(GameFramework::imagesPath() + "buttonResume.png");
    m_pButtonResume->setPos(QPOINT_TEXT_MENU_BUTTON_RESUME);
    m_textMenuResume = m_pSceneMenu->createText(QPOINT_TEXT_MENU_BUTTON_RESUME_TEXT,
                                                QString("Jouer"), 50);

    m_pButtonRestart = new Sprite(GameFramework::imagesPath() + "buttonRestart.png");
    m_pButtonRestart->setPos(QPOINT_TEXT_MENU_BUTTON_RESTART);
    m_textMenuRestart = m_pSceneMenu->createText(QPOINT_TEXT_MENU_BUTTON_RESTART_TEXT,
                                                 QString("Recommencer"), 50);
    m_textMenuRestart->hide();
    m_pButtonRestart->hide();

    m_pButtonLeave = new Sprite(GameFramework::imagesPath() + "buttonLeave.png");
    m_pButtonLeave->setPos(QPOINT_TEXT_MENU_BUTTON_LEAVE);
    m_textMenuLeave = m_pSceneMenu->createText(QPOINT_TEXT_MENU_BUTTON_LEAVE_TEXT,
                                               QString("Quitter"), 50);

    // Ajout des boutons à la scène menu.
    m_pSceneMenu->addSpriteToScene(m_pButtonResume);
    m_pSceneMenu->addSpriteToScene(m_pButtonRestart);
    m_pSceneMenu->addSpriteToScene(m_pButtonLeave);
}

//! Défini un background au menu du jeu.
//! \brief GameCore::setupBackGroundMenu
void GameCore::setupBackGroundMenu() {
    // Ajout du background au menu
    Sprite* pSpriteBackGround = new Sprite(GameFramework::imagesPath() + "backgroundCB.jpg");
    m_pSceneMenu->addSpriteToScene(pSpriteBackGround);
    pSpriteBackGround->setPos(-350,-350);
    m_pSceneMenu->createText(QPOINT_CENTER_TEXT,"Menu du jeu",100);
}

//! Création de la scène gagnante.
//! \brief GameCore::createSceneWin
void GameCore::createSceneWin() {

    // Création scène gagnante
    m_pSceneWin = m_pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_WIDTH / GameFramework::screenRatio());
    m_pWinGame = new Sprite(GameFramework::imagesPath() + "youWin.jpg");
    m_pTrophy = new Sprite (GameFramework::imagesPath() + "trophy.png");
    m_pWinGame->setPos(100, 120);
    m_pTrophy->setPos(540,75);
    m_pSceneWin->addSpriteToScene(m_pWinGame);
    m_pSceneWin->addSpriteToScene(m_pTrophy);
}

//! Création de la scène perdante.
//! \brief GameCore::createSceneLoss
void GameCore::createSceneLoss() {
    // Création scène perdante
    m_pSceneLoss = m_pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_WIDTH / GameFramework::screenRatio());
    m_pLossGame = new Sprite(GameFramework::imagesPath() + "gameover.jpg");
    m_pLossGame->setPos(0, -80);
    m_pSceneLoss->addSpriteToScene(m_pLossGame);
}

//! Création de la scène de jeu.
//! \brief GameCore::createSceneGame
void GameCore::createSceneGame() {
    // Créé la scène de base et indique au canvas qu'il faut l'afficher.
    m_pSceneGame = m_pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_WIDTH / GameFramework::screenRatio());
    m_pSceneGame->setBackgroundColor(m_colorBackGround);
}

//! Création de la scène du menu du jeu.
//! \brief GameCore::createSceneMenu
void GameCore::createSceneMenu() {
    m_pSceneMenu = m_pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_WIDTH / GameFramework::screenRatio());
}
