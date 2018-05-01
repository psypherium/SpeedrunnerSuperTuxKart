//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2004-2015 Steve Baker <sjbaker1@airmail.net>
//  Copyright (C) 2006-2015 SuperTuxKart-Team, Joerg Henrichs, Steve Baker
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_KART_HPP
#define HEADER_KART_HPP

/**
  * \defgroup karts
  * Contains classes that deal with the properties, models and physics
  * of karts.
  */

#include "LinearMath/btTransform.h"

#include "items/powerup_manager.hpp"    // For PowerupType
#include "karts/abstract_kart.hpp"
#include "utils/no_copy.hpp"

#include <SColor.h>

class AbstractKartAnimation;
class Attachment;
class btKart;
class btUprightConstraint;
class Controller;
class HitEffect;
class Item;
class KartGFX;
class KartRewinder;
class MaxSpeed;
class ParticleEmitter;
class ParticleKind;
class SFXBase;
class Shadow;
class Skidding;
class SkidMarks;
class SlipStream;
class Stars;
class TerrainInfo;

/** The main kart class. All type of karts are of this object, but with
 *  different controllers. The controllers are what turn a kart into a
 *  player kart (i.e. the controller handle input), or an AI kart (the
 *  controller runs the AI code to set steering etc).
 *  Kart has two base classes: the most important one is moveable (which
 *  is an object that is moved on the track, and has position and rotations)
 *  and TerrainInfo, which manages the terrain the kart is on.
 * \ingroup karts
 */
class Kart : public AbstractKart
{
    friend class Skidding;
protected:
    /** Offset of the graphical kart chassis from the physical chassis. */
    float m_graphical_y_offset;

    /** The coordinates of the front of the kart, used to determine when a
     *  new lap is triggered. */
    Vec3 m_xyz_front;

    /* Determines the time covered by the history size, in seconds */
    const float XYZ_HISTORY_TIME = 0.25f;

    /* Determines the number of previous XYZ positions of the kart to remember
       Initialized in the constructor and unchanged from then on */
    int m_xyz_history_size;

    /** The coordinates of the XYZ_HISTORY_SIZE previous positions */
    std::vector<Vec3> m_previous_xyz;

    float m_time_previous_counter;

    /** Is time flying activated */
    bool m_is_jumping;

protected:
    /** Handles speed increase and capping due to powerup, terrain, ... */
    MaxSpeed *m_max_speed;

    /** Stores information about the terrain the kart is on. */
    TerrainInfo *m_terrain_info;

    /** Handles the powerup of a kart. */
    Powerup *m_powerup;
    
    /** Remember the last **used** powerup type of a kart for AI purposes. */
    PowerupManager::PowerupType m_last_used_powerup;

    /** True if kart is flying (for debug purposes only). */
    bool m_flying;

    /** Set when hitting bubblegum */
    bool m_has_caught_nolok_bubblegum;

    /** Reset position. */
    btTransform  m_reset_transform;

    /** This object handles all skidding. */
    Skidding *m_skidding;

    /** The main controller of this object, used for driving. This
     *  controller is used to run the kart. It will be replaced
     *  with an end kart controller when the kart finishes the race. */
    Controller  *m_controller;

    /** This saves the original controller when the end controller is
     *  used. This is an easy solution for restarting the race, since
     *  the controller do not need to be reinitialised. */
    Controller  *m_saved_controller;

    /** Initial rank of the kart. */
    int m_initial_position;

    /** Current race position (1-num_karts). */
    int m_race_position;

    /** True if the kart wins, false otherwise. */
    bool m_race_result;

    /** True if the kart is eliminated. */
    bool m_eliminated;

    /** For stars rotating around head effect */
    Stars *m_stars_effect;

    /** True if the kart hasn't moved since 'ready-set-go' - used to
     *  determine startup boost. */
    bool         m_has_started;

    /** Maximum engine rpm's for the current gear. */
    float        m_max_gear_rpm;

    /** How long the brake key has been pressed - the longer the harder
     *  the kart will brake. */
    int          m_brake_ticks;

    /** A short time after a collision acceleration is disabled to allow
     *  the karts to bounce back*/
    int          m_bounce_back_ticks;

    /** Time a kart is invulnerable. */
    int          m_invulnerable_ticks;

    /** How long a kart is being squashed. If this is >0
     *  the kart is squashed. */
    int          m_squash_ticks;

    /** Current leaning of the kart. */
    float        m_current_lean;

    /** If > 0 then bubble gum effect is on. This is the sliding when hitting a gum on the floor, not the shield. */
    int          m_bubblegum_ticks;

    /** The torque to apply after hitting a bubble gum. */
    float        m_bubblegum_torque;

    /** True if fire button was pushed and not released */
    bool         m_fire_clicked;

    /** True if the kart has been selected to have a boosted ai */
    bool         m_boosted_ai;

    // Bullet physics parameters
    // -------------------------
    btCompoundShape          m_kart_chassis;
    btVehicleRaycaster      *m_vehicle_raycaster;
    btKart                  *m_vehicle;

     /** The amount of energy collected by hitting coins. Note that it
      *  must be float, since dt is subtraced in each timestep. */
    float         m_collected_energy;

    // Graphical effects
    // -----------------

    /** The shadow of a kart. */
    Shadow          *m_shadow;

    /** All particle effects. */
    KartGFX         *m_kart_gfx;

    /** For collisions */
    ParticleEmitter *m_collision_particles;

    /** Handles all slipstreaming. */
    SlipStream      *m_slipstream;

    /** The skidmarks object for this kart. */
    SkidMarks      *m_skidmarks;

    float           m_finish_time;
    bool            m_finished_race;

    float           m_falling_time;

    /** When a kart has its view blocked by the plunger, this variable will be
     *  > 0 the number it contains is the time left before removing plunger. */
    int           m_view_blocked_by_plunger;
    /** The current speed (i.e. length of velocity vector) of this kart. */
    float         m_speed;
    /** For camera handling an exponentially smoothened value is used, which
     *  reduces stuttering of the camera. */
    float         m_smoothed_speed;
    
    /** For smoothing engine sound**/
    float         m_last_factor_engine_sound;

    std::vector<SFXBase*> m_custom_sounds;
    int m_emitter_id = 0;
    static const int EMITTER_COUNT = 3;
    SFXBase      *m_emitters[EMITTER_COUNT];
    SFXBase      *m_engine_sound;
    SFXBase      *m_terrain_sound;
    SFXBase      *m_nitro_sound;
    /** A pointer to the previous terrain sound needs to be saved so that an
     *  'older' sfx can be finished and an abrupt end of the sfx is avoided. */
    SFXBase      *m_previous_terrain_sound;
    SFXBase      *m_skid_sound;
    SFXBuffer    *m_horn_sound;
    static const int CRASH_SOUND_COUNT = 3;
    SFXBuffer    *m_crash_sounds[CRASH_SOUND_COUNT];
    SFXBuffer    *m_goo_sound;
    SFXBuffer    *m_boing_sound;
    int          m_ticks_last_crash;
    RaceManager::KartType m_type;

    /** To prevent using nitro in too short bursts */
    int           m_min_nitro_ticks;

    void          updatePhysics(int ticks);
    void          handleMaterialSFX(const Material *material);
    void          handleMaterialGFX(float dt);
    void          updateFlying();
    void          updateSliding();
    void          updateEnginePowerAndBrakes(int ticks);
    void          updateEngineSFX(float dt);
    void          updateSpeed();
    void          updateNitro(int ticks);
    float         getActualWheelForce();
    void          playCrashSFX(const Material* m, AbstractKart *k);
    void          loadData(RaceManager::KartType type, bool animatedModel);

public:
                   Kart(const std::string& ident, unsigned int world_kart_id,
                        int position, const btTransform& init_transform,
                        PerPlayerDifficulty difficulty,
                        std::shared_ptr<RenderInfo> ri);
    virtual       ~Kart();
    virtual void   init(RaceManager::KartType type);
    virtual void   kartIsInRestNow();
    virtual void   updateGraphics(float dt) OVERRIDE;
    virtual void   createPhysics    ();
    virtual void   updateWeight     ();
    virtual float  getSpeedForTurnRadius(float radius) const;
    virtual float  getMaxSteerAngle(float speed) const;
    virtual bool   isInRest         () const;
    virtual void   applyEngineForce (float force);

    virtual void   flyUp();
    virtual void   flyDown();

    virtual void   startEngineSFX   ();
    virtual void   adjustSpeed      (float f);
    virtual void   increaseMaxSpeed(unsigned int category, float add_speed,
                                    float engine_force, float duration,
                                    float fade_out_time);
    virtual void   instantSpeedIncrease(unsigned int category, float add_max_speed,
                                    float speed_boost, float engine_force, float duration,
                                    float fade_out_time);
    virtual void   setSlowdown(unsigned int category, float max_speed_fraction,
                               int fade_in_time);
    virtual int   getSpeedIncreaseTicksLeft(unsigned int category) const;
    virtual void  setBoostAI     (bool boosted);
    virtual bool  getBoostAI     () const;
    virtual void  collectedItem(Item *item, int random_attachment);
    virtual float getStartupBoost() const;

    virtual const Material *getMaterial() const;
    virtual const Material *getLastMaterial() const;
    /** Returns the pitch of the terrain depending on the heading. */
    virtual float getTerrainPitch(float heading) const;

    virtual void   reset            ();
    virtual void   handleZipper     (const Material *m=NULL,
                                     bool play_sound=false);
    virtual void   setSquash        (float time, float slowdown);

    virtual void   crashed          (AbstractKart *k, bool update_attachments);
    virtual void   crashed          (const Material *m, const Vec3 &normal);
    virtual float  getHoT           () const;
    virtual void   update           (int ticks);
    virtual void   finishedRace     (float time, bool from_server=false);
    virtual void   setPosition      (int p);
    virtual void   beep             ();
    virtual void   showZipperFire   ();
    virtual float  getCurrentMaxSpeed() const;

    virtual bool   playCustomSFX    (unsigned int type);
    virtual void   setController(Controller *controller);
    virtual void   setXYZ(const Vec3& a);

    // ========================================================================
    // Powerup related functions.
    // ------------------------------------------------------------------------
    /** Sets a new powerup. */
    virtual void setPowerup (PowerupManager::PowerupType t, int n);
    // ------------------------------------------------------------------------
    /** Sets the last used powerup. */
    virtual void setLastUsedPowerup (PowerupManager::PowerupType t);
    // ------------------------------------------------------------------------
    /** Returns the current powerup. */
    virtual const Powerup* getPowerup() const { return m_powerup; }
    // ------------------------------------------------------------------------
    /** Returns the current powerup. */
    virtual Powerup* getPowerup() { return m_powerup; }
    // ------------------------------------------------------------------------
    /** Returns the last used powerup. */
    virtual PowerupManager::PowerupType getLastUsedPowerup() { return m_last_used_powerup; }
    // ------------------------------------------------------------------------
    /** Returns the number of powerups. */
    virtual int getNumPowerup() const;
    // ------------------------------------------------------------------------
    /** Returns a points to this kart's graphical effects. */
    virtual KartGFX* getKartGFX()               { return m_kart_gfx;         }
    // ------------------------------------------------------------------------
    /** Returns the remaining collected energy. */
    virtual float  getEnergy           () const { return m_collected_energy; }
    // ------------------------------------------------------------------------
    /** Returns the current position of this kart in the race. */
    virtual int    getPosition         () const { return m_race_position;    }
    // ------------------------------------------------------------------------
    /** Returns the coordinates of the front of the kart. This is used for
     *  determining when the lap line is crossed. */
    virtual const Vec3& getFrontXYZ() const { return m_xyz_front; }
    // ------------------------------------------------------------------------
    /** Returns the initial position of this kart. */
    virtual int    getInitialPosition  () const { return m_initial_position; }
    // ------------------------------------------------------------------------
    /** Returns the finished time for a kart. */
    virtual float  getFinishTime       () const { return m_finish_time;      }
    // ------------------------------------------------------------------------
    /** Returns true if this kart has finished the race. */
    virtual bool   hasFinishedRace     () const { return m_finished_race;    }
    // ------------------------------------------------------------------------
    /** Returns true if the kart has a plunger attached to its face. */
    virtual int getBlockedByPlungerTicks() const
                                         { return m_view_blocked_by_plunger; }
    // ------------------------------------------------------------------------
    /** Sets that the view is blocked by a plunger. The duration depends on
     *  the difficulty, see KartPorperties getPlungerInFaceTime. */
    virtual void   blockViewWithPlunger();
    // -------------------------------------------------------------------------
    /** Returns a bullet transform object located at the kart's position
        and oriented in the direction the kart is going. Can be useful
        e.g. to calculate the starting point and direction of projectiles. */
    virtual btTransform getAlignedTransform(const float customPitch=-1);
    // -------------------------------------------------------------------------
    /** Returns the color used for this kart. */
    const irr::video::SColor &getColor() const;
    // ------------------------------------------------------------------------
    /** Returns the time till full steering is reached for this kart.
     *  \param steer Current steer value (must be >=0), on which the time till
     *         full steer depends. */
    virtual float getTimeFullSteer(float steer) const;
    // ------------------------------------------------------------------------
    /** Returns the maximum steering angle for this kart, which depends on the
     *  speed. */
    virtual float getMaxSteerAngle () const
                    { return getMaxSteerAngle(getSpeed()); }
    // ------------------------------------------------------------------------
    /** Returns the skidding object for this kart (which can be used to query
     *  skidding related values). */
    virtual const Skidding *getSkidding() const { return m_skidding; }
    // ------------------------------------------------------------------------
    /** Returns the skidding object for this kart (which can be used to query
     *  skidding related values) - non-const. */
    virtual Skidding *getSkidding() { return m_skidding; }
    // ------------------------------------------------------------------------
    virtual RaceManager::KartType getType() const { return m_type; }
    // ------------------------------------------------------------------------
    /** Returns the bullet vehicle which represents this kart. */
    virtual btKart    *getVehicle() const {return m_vehicle;               }
    // ------------------------------------------------------------------------
    /** Returns the speed of the kart in meters/second. */
    virtual float        getSpeed() const {return m_speed;                 }
    // ------------------------------------------------------------------------
    /** Returns the speed of the kart in meters/second. */
    virtual float        getSmoothedSpeed() const { return m_smoothed_speed; }
    // ------------------------------------------------------------------------
    /** This is used on the client side only to set the speed of the kart
     *  from the server information.                                       */
    virtual void         setSpeed(float s) {m_speed = s;                   }
    // ------------------------------------------------------------------------
    virtual btQuaternion getVisualRotation() const;
    // ------------------------------------------------------------------------
    /** Returns the slipstream object of this kart. */
    virtual const SlipStream* getSlipstream() const {return m_slipstream; }
    // ------------------------------------------------------------------------
    /** Returns the slipstream object of this kart. */
    virtual SlipStream* getSlipstream() {return m_slipstream; }
    // ------------------------------------------------------------------------
    /** Activates a slipstream effect, atm that is display some nitro. */
    virtual void setSlipstreamEffect(float f);
    // ------------------------------------------------------------------------
    /** Returns the start transform, i.e. position and rotation. */
    const btTransform& getResetTransform() const {return m_reset_transform;}
    // ------------------------------------------------------------------------
    /** Returns the controller of this kart. */
    virtual Controller* getController() { return m_controller; }
    // ------------------------------------------------------------------------
    /** Returns the controller of this kart (const version). */
    const Controller* getController() const { return m_controller; }
    // ------------------------------------------------------------------------
    /** True if the wheels are touching the ground. */
    virtual bool isOnGround() const;
    // ------------------------------------------------------------------------
    /** Returns true if the kart is close to the ground, used to dis/enable
     *  the upright constraint to allow for more realistic explosions. */
    bool isNearGround() const;
    // ------------------------------------------------------------------------
    /** Returns true if the kart is eliminated.  */
    virtual bool isEliminated() const { return m_eliminated; }
    // ------------------------------------------------------------------------
    virtual void eliminate();
    // ------------------------------------------------------------------------
    /** Makes a kart invulnerable for a certain amount of time. */
    virtual void setInvulnerableTicks(int ticks) OVERRIDE
    { 
        m_invulnerable_ticks = ticks;
    }   // setInvulnerableTicks
    // ------------------------------------------------------------------------
    /** Returns if the kart is invulnerable. */
    virtual bool isInvulnerable() const { return m_invulnerable_ticks > 0; }
    // ------------------------------------------------------------------------
    /** Enables a kart shield protection for a certain amount of time. */
    virtual void setShieldTime(float t);
    // ------------------------------------------------------------------------
    /** Returns if the kart is protected by a shield. */
    virtual bool isShielded() const;
    // ------------------------------------------------------------------------
    /** Returns the remaining time the kart is protected by a shield. */
    virtual float getShieldTime() const;
    // ------------------------------------------------------------------------
    /** Decreases the kart's shield time. */
    virtual void decreaseShieldTime();
    // ------------------------------------------------------------------------

    /** Sets the energy the kart has collected. */
    virtual void   setEnergy(float val) { m_collected_energy = val; }
    // ------------------------------------------------------------------------
    /** Return whether nitro is being used despite the nitro button not being
     *  pressed due to minimal use time requirements
     */
    virtual bool isOnMinNitroTime() const { return m_min_nitro_ticks > 0; }
    // ------------------------------------------------------------------------
    /** Returns if the kart is currently being squashed. */
    virtual bool isSquashed() const { return m_squash_ticks >0; }
    // ------------------------------------------------------------------------
    /** Shows the star effect for a certain time. */
    virtual void showStarEffect(float t);
    // ------------------------------------------------------------------------
    /** Returns the terrain info oject. */
    virtual const TerrainInfo *getTerrainInfo() const { return m_terrain_info; }
    // ------------------------------------------------------------------------
    virtual void setOnScreenText(const wchar_t *text);
    // ------------------------------------------------------------------------
    /** Returns the normal of the terrain the kart is over atm. This is
     *  defined even if the kart is flying. */
    virtual const Vec3& getNormal() const;
    // ------------------------------------------------------------------------
    /** Returns the position 0.25s before */
    virtual const Vec3& getPreviousXYZ() const;
    // ------------------------------------------------------------------------
    /** Returns a more recent different previous position */
    virtual const Vec3& getRecentPreviousXYZ() const;
    // ------------------------------------------------------------------------
    /** For debugging only: check if a kart is flying. */
    bool isFlying() const { return m_flying;  }
    // ------------------------------------------------------------------------
    /** Returns whether this kart wins or loses. */
    virtual bool getRaceResult() const { return m_race_result;  }
    // ------------------------------------------------------------------------
    /** Set this kart race result. */
    void setRaceResult();
    // ------------------------------------------------------------------------
    /** Returns whether this kart is a ghost (replay) kart. */
    virtual bool isGhostKart() const { return false;  }
    // ------------------------------------------------------------------------
    /** Returns whether this kart is jumping. */
    virtual bool isJumping() const { return m_is_jumping; };
    // ------------------------------------------------------------------------
    SFXBase* getNextEmitter();
    // ------------------------------------------------------------------------
    virtual void playSound(SFXBuffer* buffer);
};   // Kart


#endif

/* EOF */
