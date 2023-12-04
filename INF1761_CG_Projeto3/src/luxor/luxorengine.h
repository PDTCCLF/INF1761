#include <memory>
class LuxorEngine;
using LuxorEnginePtr = std::shared_ptr<LuxorEngine>; 

#ifndef LUXOR_ENGINE_H
#define LUXOR_ENGINE_H

#include "engine.h"
#include "animation.h"

class LuxorEngine : public Engine {
  bool m_reverse;
  float m_head_angle;
  std::string m_status;
  AnimationPtr m_curr_anim;
  AnimationPtr m_stand_down_anim;
  AnimationPtr m_jump_forward_anim;
  AnimationPtr m_turn_head_anim;
  AnimationPtr m_backflip_anim;
  TransformPtr m_trf_all;
  TransformPtr m_trf_base;
  TransformPtr m_trf_haste1;
  TransformPtr m_trf_haste2;
  TransformPtr m_trf_haste3;
  TransformPtr m_trf_cupula;
  TransformPtr m_trf_lampada;
  protected:
  LuxorEngine (TransformPtr trf_all,
               TransformPtr trf_base,
               TransformPtr trf_haste1,
               TransformPtr trf_haste2,
               TransformPtr trf_haste3,
               TransformPtr trf_cupula,
               TransformPtr trf_lampada
              );
  public:
  static LuxorEnginePtr Make (TransformPtr trf_all,
                              TransformPtr trf_base,
                              TransformPtr trf_haste1,
                              TransformPtr trf_haste2,
                              TransformPtr trf_haste3,
                              TransformPtr trf_cupula,
                              TransformPtr trf_lampada
                              );
  virtual ~LuxorEngine ();
  bool StandUp ();
  bool StandDown ();
  bool JumpForward ();
  bool JumpBackward ();
  void TurnHead (float angle);
  bool TurnHeadDown ();
  bool BackflipForward ();
  bool BackflipBackward ();
  virtual void Update (float dt);
private:
  void CreateStandDownAnimation ();
  void CreateJumpForwardAnimation ();
  void CreateTurnHeadDownAnimation ();
  void CreateBackflipAnimation ();
};

#endif