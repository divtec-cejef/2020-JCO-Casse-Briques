/*! \mainpage Game Framework
 *
 * \author RBR
 * \section intro_sec Introduction
 *
 * Ce projet est la création d'un jeu simple en 2D.
 * Il est plutôt orienté jeu d'arcade.
 *
 * Il prend en charge l'organisation des classes nécessaires à l'affichage
 * d'une surface de jeu en deux dimensions.
 *
 * En plus de fournir l'architecture de base, ce projet démontre quelques
 * fonctionnalités rudimentaires :
 *
 * - Détection de collisions (voir BouncingSpriteHandler).
 * - Utilisation d'un tick (voir gamecore).
 * - Détection des touches et clics appuyés (voir gamecore à l'aide de keyPressed().
 * - Gère le rebond de la balle (voir BouncingSpriteHandler).
 * - Utilisation de plusieurs scènes (surfaces de jeu), au moyen de GameCanvas::createScene()
 *   et GameCanvas::setCurrentScene().
 *
 * Ces différentes fonctionnalités sont décrites dans GameCore.
 *
 * Les méthodes suivantes sont utilisées dans GameCore :
 * - GameCanvas::startTick() : Cette fonction démarre le timing, ce qui a pour effet
 * d'appeler la fonction GameCore::tick() de façon régulière.
 * - GameCanvas::stopTick() : Cette fonction interrompt le timing. La fonction GameCore::tick() n'est plus appelée.
 *
 * - GameScene::setWidth() : Permet de déterminer la largeur, en pixels, de la surface de jeu. La fonction GameScene::width() permet de la relire.
 * - GameScene::setHeight() : Permet de déterminer la hauteur, en pixels, de la surface de jeu. La fonction GameScene::height() permet de la relire.
 * - GameScene::addSpriteToScene() : Cette fonction intègre le sprite donné à la scène. La scène en prend la propriété
 *   et se chargera donc de le détruire lorsque l'application se termine.
 * - GameScene::collidingSprites() : Cette fonction permet d'obtenir une liste de sprites en collision avec un sprite donné.
 * - GameScene::spriteAt() : Cette fonction permet de récupérer le sprite se trouvant à la position donnée.
 * - GameScene::createText() : Cette fonction ajoute à la scène un texte et retourne un pointeur permettant de manipuler ce texte.
 */

#include "mainfrm.h"

#include <QApplication>

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainFrm w;
    MainFrm cb;

    cb.show();

    //w.show();

    // Pour un mode d'affichage fenêtré, plein écran
    // w.showMaximized();

    // Pour un mode d'affichage non-fenêtré, plein écran
    // w.showFullScreen();

    return a.exec();
}
