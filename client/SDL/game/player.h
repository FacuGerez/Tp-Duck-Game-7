#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../../../common/duckState/duckState.h"

#include "image.h"
#include "enums.h"

class Player {
private:
    SDL_Renderer* rend;
    SDL_RendererFlip flip;
    std::string file;

    uint tiles;

    std::unordered_map<DuckState, std::vector<std::shared_ptr<Image>>> ducks;
    std::vector<std::shared_ptr<Image>> wings;

    DuckState state;

    Side side;

    ProjectilesId::ProjectileId weaponId;

    bool shooted;
    bool armorOn;
    bool helmetOn;
    int walk;
    bool flapup;

    double weaponAngle;

    uint width;
    uint height;

    std::shared_ptr<Image> duck;
    std::shared_ptr<Image> wing;

    std::shared_ptr<Image> _armor;
    std::shared_ptr<Image> _hombro;
    std::shared_ptr<Image> _helmet;
    std::shared_ptr<Image> _weapon;
    std::shared_ptr<Image> _shootEfect;

    void initializeWingImage(WingState wingState);
    void updateWing(int x, int y);
    void initialiceDuckImages(DuckState state);

public:
    Player(SDL_Renderer* rend, Color color);

    // El tamaño original de los png son del pato 24x24 y del ala 15x15
    void defineSize(int height, int width);

    // Actualizar posicion e imagen
    void update(int x, int y, DuckState newState, Side side);
    void standing();

    // Vacia el inventario
    void dropEverithing();

    // Agarrar un arma
    void weapon(std::shared_ptr<Image> weapon, ProjectilesId::ProjectileId weaponId);
    // Devolver el arma
    bool dropWeapon();
    // Disparar arma
    ProjectilesId::ProjectileId shoot(std::shared_ptr<Image> shootEfect);

    // Agarrar armadura
    void armor(std::shared_ptr<Image> armor, std::shared_ptr<Image> hombro);
    // Droppear armadura
    bool dropArmor();

    // Agarrar/Dropear casco
    void helmet(std::shared_ptr<Image> helmet);
    // Droppear casco
    bool dropHelmet();

    // Dibujar jugador
    void fill();

    // Devolver posicion del jugador
    std::pair<int, int> getPosition() const;

    // Redimensionar el jugador
    void set_tiles(uint tiles);

    // Path
    std::string get_file() const;

};
