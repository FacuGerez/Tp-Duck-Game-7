#ifndef DESCRIPCIONLOBBY_H
#define DESCRIPCIONLOBBY_H
#include <cstdint>
#include <string>

struct DescripcionPlayer {
    std::string nombre;
    uint8_t color;
    uint8_t is_ready;
    // agregar mas en caso de ser necesario

    ~DescripcionPlayer() = default;
};

/*
 * Clase que representa la descripcion de un lobby
 */
struct DescripcionLobby {
    uint8_t idLobby;
    std::string nombreLobby;
    uint8_t cantidadJugadores;  // 1 o 2 por ahora (tanto online como local)
    uint8_t maxJugadores;  // 2 a 6 por ahora
    /*
     * Destructor del mensaje del lobby
     */
    ~DescripcionLobby() = default;
};
#endif
