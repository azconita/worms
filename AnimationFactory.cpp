/*
 * AnimationFactory.cpp
 *
 *  Created on: 3 jun. 2018
 *      Author: jaz
 */

#include "AnimationFactory.h"


    std::vector<Picture> AnimationFactory::get_little_beams(){
        std::vector<Picture> little_beams;
        Colour colorkey_beam(BEAM_R,BEAM_G,BEAM_B);
        
        Picture beam0(BEAM3_0, colorkey_beam,BEAM_COLUMNS,BEAM_ROWS);
        little_beams.push_back(beam0);

        Picture beam22(BEAM3_22, colorkey_beam,BEAM_COLUMNS,BEAM_ROWS);
        little_beams.push_back(beam22);

        Picture beam45(BEAM3_45, colorkey_beam,BEAM_COLUMNS,BEAM_ROWS);
        little_beams.push_back(beam45);

        Picture beam65(BEAM3_65, colorkey_beam,BEAM_COLUMNS,BEAM_ROWS);
        little_beams.push_back(beam65);

        Picture beam90(BEAM3_90, colorkey_beam,BEAM_COLUMNS,BEAM_ROWS);
        little_beams.push_back(beam90);

        return little_beams;
    }


    std::vector<Picture> AnimationFactory::get_big_beams(){
        std::vector<Picture> big_beams;
        Colour colorkey_beam(BEAM_R,BEAM_G,BEAM_B);
        
        Picture beam0(BEAM6_0, colorkey_beam,BEAM_COLUMNS,BEAM_ROWS);
        big_beams.push_back(beam0);

        Picture beam22(BEAM6_22, colorkey_beam,BEAM_COLUMNS,BEAM_ROWS);
        big_beams.push_back(beam22);

        Picture beam45(BEAM6_45, colorkey_beam,BEAM_COLUMNS,BEAM_ROWS);
        big_beams.push_back(beam45);

        Picture beam65(BEAM6_65, colorkey_beam,BEAM_COLUMNS,BEAM_ROWS);
        big_beams.push_back(beam65);

        Picture beam90(BEAM6_90, colorkey_beam,BEAM_COLUMNS,BEAM_ROWS);
        big_beams.push_back(beam90);

        return big_beams;
    }



std::map<State,Animation> AnimationFactory::get_wormsAnimations(){
    std::map<State,Animation> animations;

    Animation worm_walk = AnimationFactory::get_worm_walk();
    animations.insert(std::pair<State,Animation>(Still,worm_walk));
    animations.insert(std::pair<State,Animation>(Walk,worm_walk));

    Animation worm_fall = AnimationFactory::get_worm_fall();
    animations.insert(std::pair<State,Animation>(Fall,worm_fall));

    Animation worm_jump = AnimationFactory::get_worm_jump();
    animations.insert(std::pair<State,Animation>(Jump_state,worm_jump));

    Animation worm_jump_back = AnimationFactory::get_worm_jump_back();
    animations.insert(std::pair<State,Animation>(Jump_back_state,worm_jump_back));

    Animation worm_missile = AnimationFactory::get_worm_missile();
    animations.insert(std::pair<State,Animation>(Worm_missile,worm_missile));

    Animation worm_banana = AnimationFactory::get_worm_banana();
    animations.insert(std::pair<State,Animation>(Worm_banana,worm_banana));

    Animation worm_bat = AnimationFactory::get_worm_bat();
    animations.insert(std::pair<State,Animation>(Worm_bat,worm_bat));

    Animation worm_green_granade = AnimationFactory::get_worm_green_granade();
    animations.insert(std::pair<State,Animation>(Worm_green_granade,worm_green_granade));

    Animation worm_red_granade = AnimationFactory::get_worm_red_granade();
    animations.insert(std::pair<State,Animation>(Worm_red_granade,worm_red_granade));

    Animation worm_holy_granade = AnimationFactory::get_worm_holy_granade();
    animations.insert(std::pair<State,Animation>(Worm_holy_granade,worm_holy_granade));

    Animation worm_teletrans = AnimationFactory::get_worm_teletrans();
    animations.insert(std::pair<State,Animation>(Worm_teletrans,worm_teletrans));

    Animation worm_air_attack = AnimationFactory::get_worm_air_attack();
    animations.insert(std::pair<State,Animation>(Worm_air_attack,worm_air_attack));

    Animation worm_dynamite = AnimationFactory::get_worm_dynamite();
    animations.insert(std::pair<State,Animation>(Worm_dynamite,worm_dynamite));

    return animations;

}


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

    Animation AnimationFactory::get_worm_jump_back(){
        Colour colorkey(WORM_JUMP_BACK_R,WORM_JUMP_BACK_G,WORM_JUMP_BACK_B);
        Animation worm(WORM_JUMP_BACK,colorkey,WORM_JUMP_BACK_COLUMNS,WORM_JUMP_BACK_ROWS);
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


std::map<Weapon_Name,Animation> AnimationFactory::get_weapons(){
    std::map<Weapon_Name,Animation> animations;

    Animation bazooka = AnimationFactory::get_bazooka();
    animations.insert(std::pair<Weapon_Name,Animation>(W_Bazooka, bazooka));

    Animation mortar = AnimationFactory::get_mortar();
    animations.insert(std::pair<Weapon_Name,Animation>(Mortar,mortar));

    Animation banana = AnimationFactory::get_banana();
    animations.insert(std::pair<Weapon_Name,Animation>(Banana,banana));

    Animation green_granade = AnimationFactory::get_green_granade();
    animations.insert(std::pair<Weapon_Name,Animation>(Green_Grenade,green_granade));

    Animation red_granade = AnimationFactory::get_red_granade();
    animations.insert(std::pair<Weapon_Name,Animation>(Red_Grenade,red_granade));

    Animation holy_granade = AnimationFactory::get_holy_granade();
    animations.insert(std::pair<Weapon_Name,Animation>(Holy_Grenade,holy_granade));

    Animation air_attack = AnimationFactory::get_air_attack();
    animations.insert(std::pair<Weapon_Name,Animation>(W_Air_Attack,air_attack));

    Animation dynamite = AnimationFactory::get_dynamite();
    animations.insert(std::pair<Weapon_Name,Animation>(Dynamite,dynamite));

    Animation explosion = AnimationFactory::get_explosion();
    animations.insert(std::pair<Weapon_Name,Animation>(Explosion,explosion));
    
    return animations;
}


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
        Colour colorkey(BIG_EXPLOSION_R,BIG_EXPLOSION_G,BIG_EXPLOSION_B);
        Animation worm(BIG_EXPLOSION,colorkey,BIG_EXPLOSION_COLUMNS,BIG_EXPLOSION_ROWS);
        return worm;
    }





  

AnimationFactory::~AnimationFactory() {
	// TODO Auto-generated destructor stub
}

