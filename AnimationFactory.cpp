/*
 * AnimationFactory.cpp
 *
 *  Created on: 3 jun. 2018
 *      Author: jaz
 */

#include "AnimationFactory.h"


     Animation AnimationFactory::get_worm_walk(){
        Colour colorkey(WORM_WALK_R,WORM_WALK_G,WORM_WALK_B);
        Animation worm(WORM_WALK,colorkey,WORM_WALK_COLUMNS,WORM_WALK_ROWS);
        return worm;
    }

     Animation AnimationFactory::get_worm_jump(){
        Colour colorkey(WORM_JUMP_R,WORM_JUMP_G,WORM_JUMP_B);
        Animation worm(WORM_JUMP,colorkey,WORM_JUMP_COLUMNS,WORM_JUMP_ROWS);
        return worm;

    }

     Animation AnimationFactory::get_worm_fall(){
        Colour colorkey(WORM_FALL_R,WORM_FALL_G,WORM_JUMP_B);
        Animation worm(WORM_FALL,colorkey,WORM_FALL_COLUMNS,WORM_FALL_ROWS);
        return worm;

    }

     Animation AnimationFactory::get_worm_missile(){
        Colour colorkey(WORM_MISSILE_R,WORM_MISSILE_G,WORM_MISSILE_B);
        Animation worm(WORM_MISSILE,colorkey,WORM_MISSILE_COLUMNS,WORM_MISSILE_ROWS);
        return worm;
    }

     Animation AnimationFactory::get_worm_bat(){
        Colour colorkey(WORM_BAT_R,WORM_BAT_G,WORM_BAT_B);
        Animation worm(WORM_BAT,colorkey,WORM_BAT_COLUMNS,WORM_BAT_ROWS);
        return worm;
    }

     Animation AnimationFactory::get_worm_banana(){
        Colour colorkey(WORM_BANANA_R,WORM_BANANA_G,WORM_BANANA_B);
        Animation worm(WORM_BANANA,colorkey,WORM_BANANA_COLUMNS,WORM_MISSILE_ROWS);
        return worm;
    }

     Animation AnimationFactory::get_worm_green_granade(){
        Colour colorkey(WORM_GREEN_GRANADE_R,WORM_GREEN_GRANADE_G,WORM_GREEN_GRANADE_B);
        Animation worm(WORM_GREEN_GRANADE,colorkey,WORM_GREEN_GRANADE_COLUMNS,WORM_GREEN_GRANADE_ROWS);
        return worm;
    }

     Animation AnimationFactory::get_worm_red_granade(){
        Colour colorkey(WORM_RED_GRANADE_R,WORM_RED_GRANADE_G,WORM_RED_GRANADE_B);
        Animation worm(WORM_RED_GRANADE,colorkey,WORM_RED_GRANADE_COLUMNS,WORM_RED_GRANADE_ROWS);
        return worm;
    }

     Animation AnimationFactory::get_worm_holy_granade(){
        Colour colorkey(WORM_HOLY_GRANADE_R,WORM_HOLY_GRANADE_G,WORM_HOLY_GRANADE_B);
        Animation worm(WORM_HOLY_GRANADE,colorkey,WORM_HOLY_GRANADE_COLUMNS,WORM_HOLY_GRANADE_ROWS);
        return worm;
    }

     Animation AnimationFactory::get_worm_teletrans(){
        Colour colorkey(WORM_TELETRANS_R,WORM_TELETRANS_G,WORM_TELETRANS_B);
        Animation worm(WORM_TELETRANS,colorkey,WORM_TELETRANS_COLUMNS,WORM_TELETRANS_ROWS);
        return worm;
    }

     Animation AnimationFactory::get_worm_air_attack(){
        Colour colorkey(WORM_AIR_ATTACK_R,WORM_AIR_ATTACK_G,WORM_AIR_ATTACK_B);
        Animation worm(WORM_AIR_ATTACK,colorkey,WORM_AIR_ATTACK_COLUMNS,WORM_AIR_ATTACK_ROWS);
        return worm;
    }

     Animation AnimationFactory::get_worm_dynamite(){
        Colour colorkey(WORM_DYNAMITE_R,WORM_DYNAMITE_G,WORM_DYNAMITE_B);
        Animation worm(WORM_DYNAMITE,colorkey,WORM_DYNAMITE_COLUMNS,WORM_DYNAMITE_ROWS);
        return worm;
    }

    //-----------------------Armas --------------------------------------------------


      Animation AnimationFactory::get_bazooka(){
        Colour colorkey(BAZOOKA_R,BAZOOKA_G,BAZOOKA_B);
        Animation worm(BAZOOKA,colorkey,BAZOOKA_COLUMNS,BAZOOKA_ROWS);
        return worm;
    }

        Animation AnimationFactory::get_mortar(){
        Colour colorkey(MORTAR_R,MORTAR_G,MORTAR_B);
        Animation worm(MORTAR,colorkey,MORTAR_COLUMNS,MORTAR_ROWS);
        return worm;
    }

      Animation AnimationFactory::get_banana(){
        Colour colorkey(BANANA_R,BANANA_G,BANANA_B);
        Animation worm(BANANA,colorkey,BANANA_COLUMNS,BANANA_ROWS);
        return worm;
    }

     Animation AnimationFactory::get_green_granade(){
        Colour colorkey(GREEN_GRANADE_R,GREEN_GRANADE_G,GREEN_GRANADE_B);
        Animation worm(GREEN_GRANADE,colorkey,GREEN_GRANADE_COLUMNS,GREEN_GRANADE_ROWS);
        return worm;
    }

     Animation AnimationFactory::get_red_granade(){
        Colour colorkey(RED_GRANADE_R,RED_GRANADE_G,RED_GRANADE_B);
        Animation worm(RED_GRANADE,colorkey,RED_GRANADE_COLUMNS,RED_GRANADE_ROWS);
        return worm;
    }

     Animation AnimationFactory::get_holy_granade(){
        Colour colorkey(HOLY_GRANADE_R,HOLY_GRANADE_G,HOLY_GRANADE_B);
        Animation worm(HOLY_GRANADE,colorkey,HOLY_GRANADE_COLUMNS,HOLY_GRANADE_ROWS);
        return worm;
    }


     Animation AnimationFactory::get_air_attack(){
        Colour colorkey(AIR_ATTACK_R,AIR_ATTACK_G,AIR_ATTACK_B);
        Animation worm(AIR_ATTACK,colorkey,AIR_ATTACK_COLUMNS,AIR_ATTACK_ROWS);
        return worm;
    }

     Animation AnimationFactory::get_dynamite(){
        Colour colorkey(DYNAMITE_R,DYNAMITE_G,DYNAMITE_B);
        Animation worm(DYNAMITE,colorkey,DYNAMITE_COLUMNS,DYNAMITE_ROWS);
        return worm;
    }

      Animation AnimationFactory::get_explosion(){
        Colour colorkey(EXPLOSION_R,EXPLOSION_G,EXPLOSION_B);
        Animation worm(EXPLOSION,colorkey,EXPLOSION_COLUMNS,EXPLOSION_ROWS);
        return worm;
    }

/*
    std::map<Picture> AnimationFactory::get_little_beam(){
        std::vector<Picture> little_beams;
        Colour colorkey_beam(BEAM_R,BEAM_G,BEAM_B);
        
        beam0 = Picture beam(BEAM3_0, colorkey_beam,BEAM_COLUMNS,BEAM_ROWS);
        little_beams.push_back(beam0);

        /*beam1 = Picture beam(BEAM3_22, colorkey_beam,BEAM_COLUMNS,BEAM_ROWS);
        little_beams.push_back(beam0);

        beam2 = Picture beam(BEAM3_45, colorkey_beam,BEAM_COLUMNS,BEAM_ROWS);
        little_beams.push_back(beam0);

        beam3 = Picture beam(BEAM3_65, colorkey_beam,BEAM_COLUMNS,BEAM_ROWS);
        little_beams.push_back(beam0);

        beam0 = Picture beam(BEAM3_90, colorkey_beam,BEAM_COLUMNS,BEAM_ROWS);
        little_beams.push_back(beam0);

        return little_beams;
    }


    std::map<Picture> AnimationFactory::get_big_beam(){
        std::vector<Picture> big_beams;
        Colour colorkey_beam(BEAM_R,BEAM_G,BEAM_B);
        
        beam0 = Picture beam(BEAM6_0, colorkey_beam,BEAM_COLUMNS,BEAM_ROWS);
        little_beams.push_back(beam0);

        /*beam1 = Picture beam(BEAM6_22, colorkey_beam,BEAM_COLUMNS,BEAM_ROWS);
        little_beams.push_back(beam0);

        beam2 = Picture beam(BEAM6_45, colorkey_beam,BEAM_COLUMNS,BEAM_ROWS);
        little_beams.push_back(beam0);

        beam3 = Picture beam(BEAM6_65, colorkey_beam,BEAM_COLUMNS,BEAM_ROWS);
        little_beams.push_back(beam0);

        beam0 = Picture beam(BEAM6_90, colorkey_beam,BEAM_COLUMNS,BEAM_ROWS);
        little_beams.push_back(beam0);

        return big_beams;
    }

*/

  

AnimationFactory::~AnimationFactory() {
	// TODO Auto-generated destructor stub
}

