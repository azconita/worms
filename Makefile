#como compilabasmos hasta ahora:

# > g++ -std=c++11 main.cpp StageLoader.cpp Entity.cpp WeaponExplosionListener.cpp Projectile.cpp stage.cpp Beam.cpp Worm.cpp Constants.cpp Weapon.cpp Bazooka.cpp -lBox2D -lSDL -lSDL_image -lyaml-cpp -lSDL_ttf  -lSDL_gfx -g

# Nombre del ejecutable.
target = tp

# Extensión de los archivos a compilar (c para C, cpp o cc o cxx para C++).
extension = cpp

# Archivos con el código fuente que componen el ejecutable de cleinte y servidor
# No poner cabeceras (.h).
fuentes_client = Camera.cpp client-main.cpp Client.cpp Actioner.cpp Animation.cpp AnimationFactory.cpp Colour.cpp EventController.cpp GraphicDesigner.cpp Picture.cpp WaterAnimation.cpp WormAnimation.cpp
fuentes_server = server-main.cpp Server.cpp Constants.cpp Beam.cpp Entity.cpp StageLoader.cpp Weapon.cpp WeaponExplosionListener.cpp Worm.cpp Stage.cpp Player.cpp TimerStage.cpp Game.cpp common_thread.cpp TurnHelper.cpp
fuentes_common = Lock.cpp Socket.cpp Error.cpp Logger.cpp Dtos.cpp

# Descomentar si se quiere ver como se invoca al compilador
verbose = si

# Si usa funciones de math.h, descomentar (quitar el '#' a) la siguiente línea.
#math = si

# Si usa threads, descomentar (quitar el '#' a) la siguiente línea.
threads = si

#librerias utilizadas
box2d = si
sdl = si
yaml = si


# CONFIGURACION "AVANZADA"
###########################

# Opciones para el compilador C/C++ para tratamiento de errores y warnings.
# habria que agregar -Wall -Werror !!!!!!!!
CFLAGS = -pedantic -pedantic-errors

# Para optimizar el binario resultante lo mejor posible
CFLAGS += -O3

# Para valgrind o debug
CFLAGS += -ggdb -DDEBUG -fno-inline

# Opciones del enlazador.
#LDFLAGS =

# Estandar de C a usar
CSTD = c99

# Estandar de C++ a usar
CXXSTD = c++11

# Si se quiere compilar estaticamente, descomentar la siguiente linea
#static = si


# VARIABLES CALCULADAS A PARTIR DE LA CONFIGURACION
####################################################

# Linkea con libm de ser necesario.
ifdef math
LDFLAGS += -lm
endif

#linkea con todas las librerias qeu queremos

ifdef box2d
LDFLAGS += -lBox2D 
endif

ifdef sdl
LDFLAGS += -lSDL 
LDFLAGS += -lSDL_image 
LDFLAGS += -lSDL_ttf 
endif


ifdef yaml
LDFLAGS += -lyaml-cpp 
endif


# Linkea con threads de ser necesario. Permite el uso de pthread en C y C++. Permite el uso de built-in threads en C++.
ifdef threads
LDFLAGS += -pthread
endif

ifdef static
LDFLAGS += -static
endif

# Se reutilizan los flags de C para C++ también
CXXFLAGS += $(CFLAGS)

# Se usa enlazador de c++ si es código no C.
ifeq ($(extension), c)
CFLAGS += -std=$(CSTD)
LD = $(CC)
else
CXXFLAGS += -std=$(CXXSTD)
LD = $(CXX)
endif


# Se usa enlazador de C++ si es código no C.
ifeq ($(extension), c)
CFLAGS += -std=$(CSTD)
LD = $(CC)
else
CXXFLAGS += -std=$(CXXSTD)
LD = $(CXX)
endif

# Si no especifica archivos, tomo todos.
fuentes_client ?= $(wildcard client*.$(extension))
fuentes_server ?= $(wildcard server*.$(extension))
fuentes_common ?= $(wildcard common*.$(extension))
directorios = $(shell find . -type d -regex '.*\w+')

occ := $(CC)
ocxx := $(CXX)
orm := $(RM)
old := $(LD)
ifdef verbose
RM := $(RM) -v
else
CC =  @echo "  CC  $@"; $(occ)
CXX = @echo "  CXX $@"; $(ocxx)
RM =  @echo "  CLEAN"; $(orm)
LD =  @echo "  LD  $@"; $(old)
endif


# REGLAS
#########

.PHONY: all clean

all: server client

o_common_files = $(patsubst %.$(extension),%.o,$(fuentes_common))
o_client_files = $(patsubst %.$(extension),%.o,$(fuentes_client))
o_server_files = $(patsubst %.$(extension),%.o,$(fuentes_server))

client: $(o_common_files) $(o_client_files)
	@if [ -z "$(o_client_files)" ]; \
	then \
		echo "No hay archivos de entrada en el directorio actual para el cliente. Recuerde que los archivos deben respetar la forma 'client*.$(extension)' y que no se aceptan directorios anidados."; \
		if [ -n "$(directorios)" ]; then echo "Directorios encontrados: $(directorios)"; fi; \
		false; \
	fi >&2
	$(LD) $(o_common_files) $(o_client_files) -o client $(LDFLAGS)

server: $(o_common_files) $(o_server_files)
	@if [ -z "$(o_server_files)" ]; \
	then \
		echo "No hay archivos de entrada en el directorio actual para el servidor. Recuerde que los archivos deben respetar la forma 'server*.$(extension)' y que no se aceptan directorios anidados."; \
		if [ -n "$(directorios)" ]; then echo "Directorios encontrados: $(directorios)"; fi; \
		false; \
	fi >&2
	$(LD) $(o_common_files) $(o_server_files) -o server $(LDFLAGS)

clean:
	$(RM) -f $(o_common_files) $(o_client_files) $(o_server_files) client server

