#include "stdafx.h"
#include "spell.h"
#include "effect.h"
#include "creature.h"
#include "player_message.h"
#include "creature_name.h"
#include "creature_factory.h"
#include "sound.h"
#include "lasting_effect.h"
#include "effect_type.h"
#include "furniture_type.h"
#include "attr_type.h"
#include "attack_type.h"

const string& Spell::getName() const {
  return name;
}

bool Spell::isDirected() const {
  return effect->contains<DirEffectType>();
}

bool Spell::hasEffect(EffectType t) const {
  return effect->getReferenceMaybe<EffectType>() == t;
}

bool Spell::hasEffect(DirEffectType t) const {
  return effect->getReferenceMaybe<DirEffectType>() == t;
}

EffectType Spell::getEffectType() const {
  return *effect->getReferenceMaybe<EffectType>();
}

DirEffectType Spell::getDirEffectType() const{
  return *effect->getReferenceMaybe<DirEffectType>();
}

int Spell::getDifficulty() const {
  return difficulty;
}

Spell::Spell(const string& n, EffectType e, int diff, SoundId s, CastMessageType msg)
    : name(n), effect(e), difficulty(diff), castMessageType(msg), sound(s) {
}

Spell::Spell(const string& n, DirEffectType e, int diff, SoundId s, CastMessageType msg)
    : name(n), effect(e), difficulty(diff), castMessageType(msg), sound(s) {
}

SoundId Spell::getSound() const {
  return sound;
}

string Spell::getDescription() const {
  return effect->visit([](const auto& e) { return Effect::getDescription(e); });
}

void Spell::addMessage(WCreature c) {
  switch (castMessageType) {
    case CastMessageType::STANDARD:
      c->playerMessage("You cast " + getName());
      c->monsterMessage(c->getName().the() + " casts a spell");
      break;
    case CastMessageType::AIR_BLAST:
      c->playerMessage("You create an air blast!");
      c->monsterMessage(c->getName().the() + " creates an air blast!");
      break;
  }
}

void Spell::init() {
  set(SpellId::HEAL_SELF, new Spell("heal self", EffectId::HEAL, 30, SoundId::SPELL_HEALING));
  set(SpellId::SUMMON_INSECTS, new Spell("summon insects", EffectType(EffectId::SUMMON, CreatureId::FLY), 30,
        SoundId::SPELL_SUMMON_INSECTS));
  set(SpellId::DECEPTION, new Spell("deception", EffectId::DECEPTION, 60, SoundId::SPELL_DECEPTION));
  set(SpellId::SPEED_SELF, new Spell("haste self", {EffectId::LASTING, LastingEffect::SPEED}, 60,
        SoundId::SPELL_SPEED_SELF));
  set(SpellId::DAM_BONUS, new Spell("damage", {EffectId::LASTING, LastingEffect::DAM_BONUS}, 90,
        SoundId::SPELL_STR_BONUS));
  set(SpellId::DEF_BONUS, new Spell("defense", {EffectId::LASTING, LastingEffect::DEF_BONUS}, 90,
        SoundId::SPELL_DEX_BONUS));
  set(SpellId::STUN_RAY, new Spell("stun ray",  DirEffectType(4, DirEffectId::CREATURE_EFFECT,
      EffectType(EffectId::LASTING, LastingEffect::STUNNED)) , 60, SoundId::SPELL_STUN_RAY));
  set(SpellId::HEAL_OTHER, new Spell("heal other",  DirEffectType(1, DirEffectId::CREATURE_EFFECT,
      EffectId::HEAL) , 6, SoundId::SPELL_HEALING));
  set(SpellId::MAGIC_SHIELD, new Spell("magic shield", {EffectId::LASTING, LastingEffect::MAGIC_SHIELD}, 100,
        SoundId::SPELL_MAGIC_SHIELD));
  set(SpellId::FIRE_SPHERE_PET, new Spell("fire sphere", EffectType(EffectId::SUMMON, CreatureId::FIRE_SPHERE), 20,
        SoundId::SPELL_FIRE_SPHERE_PET));
  set(SpellId::TELEPORT, new Spell("escape", EffectId::TELEPORT, 80, SoundId::SPELL_TELEPORT));
  set(SpellId::INVISIBILITY, new Spell("invisibility", {EffectId::LASTING, LastingEffect::INVISIBLE}, 150,
        SoundId::SPELL_INVISIBILITY));
  set(SpellId::BLAST, new Spell("blast", DirEffectType(4, DirEffectId::BLAST), 100, SoundId::SPELL_BLAST));
  set(SpellId::MAGIC_MISSILE, new Spell("magic missile", DirEffectType(4, DirEffectId::CREATURE_EFFECT,
      EffectType {EffectId::DAMAGE, DamageInfo{AttrType::SPELL_DAMAGE, AttackType::SPELL}}), 3, SoundId::SPELL_BLAST));
  set(SpellId::CIRCULAR_BLAST, new Spell("circular blast", EffectId::CIRCULAR_BLAST, 150, SoundId::SPELL_AIR_BLAST,
        CastMessageType::AIR_BLAST));
  set(SpellId::SUMMON_SPIRIT, new Spell("summon spirits", EffectType(EffectId::SUMMON, CreatureId::SPIRIT), 150,
        SoundId::SPELL_SUMMON_SPIRIT));
  set(SpellId::CURE_POISON, new Spell("cure poisoning", EffectId::CURE_POISON, 150, SoundId::SPELL_CURE_POISON));
  set(SpellId::METEOR_SHOWER, new Spell("meteor shower", {EffectId::PLACE_FURNITURE, FurnitureType::METEOR_SHOWER}, 150,
        SoundId::SPELL_METEOR_SHOWER));
  set(SpellId::PORTAL, new Spell("portal", {EffectId::PLACE_FURNITURE, FurnitureType::PORTAL}, 150, SoundId::SPELL_PORTAL));
  set(SpellId::SUMMON_ELEMENT, new Spell("summon element", EffectId::SUMMON_ELEMENT, 5, SoundId::SPELL_SUMMON_SPIRIT));
}

optional<int> Spell::getLearningExpLevel() const {
  switch (getId()) {
    case SpellId::HEAL_SELF: return 1;
    case SpellId::SUMMON_INSECTS: return 2;
    case SpellId::HEAL_OTHER: return 3;
    case SpellId::MAGIC_MISSILE: return 4;
    case SpellId::DECEPTION: return 4;
    case SpellId::TELEPORT: return 4;
    case SpellId::SPEED_SELF: return 5;
    case SpellId::STUN_RAY: return 6;
    case SpellId::CURE_POISON: return 6;
    case SpellId::MAGIC_SHIELD: return 7;
    case SpellId::BLAST: return 7;
    case SpellId::CIRCULAR_BLAST: return 7;
    case SpellId::DEF_BONUS: return 8;
    case SpellId::SUMMON_ELEMENT: return 8;
    case SpellId::DAM_BONUS: return 9;
    case SpellId::FIRE_SPHERE_PET: return 10;
    case SpellId::METEOR_SHOWER: return 11;
    case SpellId::INVISIBILITY: return 12;
    default:
      return none;
  }
};

