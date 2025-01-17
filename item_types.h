#pragma once

#include "util.h"
#include "weapon_info.h"
#include "item_prefix.h"
#include "item_upgrade_info.h"
#include "view_id.h"
#include "tech_id.h"
#include "custom_item_id.h"
#include "furniture_type.h"
#include "creature_id.h"
#include "minion_trait.h"

#define ITEM_TYPE_INTERFACE\
  ItemAttributes getAttributes(const ContentFactory*) const

#define SIMPLE_ITEM(Name) \
  struct Name : public EmptyStruct<Name> { \
    ITEM_TYPE_INTERFACE;\
  }

class ItemType;

namespace ItemTypes {
struct Intrinsic {
  ViewId SERIAL(viewId);
  string SERIAL(name);
  int SERIAL(damage);
  WeaponInfo SERIAL(weaponInfo);
  SERIALIZE_ALL(viewId, name, damage, weaponInfo)
  ITEM_TYPE_INTERFACE;
};
struct Scroll {
  Effect SERIAL(effect);
  SERIALIZE_ALL(effect)
  ITEM_TYPE_INTERFACE;
};
struct Potion {
  Effect SERIAL(effect);
  SERIALIZE_ALL(effect)
  ITEM_TYPE_INTERFACE;
};
struct Mushroom {
  Effect SERIAL(effect);
  SERIALIZE_ALL(effect)
  ITEM_TYPE_INTERFACE;
};
struct Amulet {
  LastingEffect SERIAL(lastingEffect);
  SERIALIZE_ALL(lastingEffect)
  ITEM_TYPE_INTERFACE;
};
struct Ring {
  LastingEffect SERIAL(lastingEffect);
  SERIALIZE_ALL(lastingEffect)
  ITEM_TYPE_INTERFACE;
};
struct Glyph {
  ItemUpgradeInfo SERIAL(rune);
  SERIALIZE_ALL(rune)
  ITEM_TYPE_INTERFACE;
};
struct Balsam {
  Effect SERIAL(effect);
  SERIALIZE_ALL(effect)
  ITEM_TYPE_INTERFACE;
};
struct TechBook {
  TechId SERIAL(techId);
  SERIALIZE_ALL(techId)
  ITEM_TYPE_INTERFACE;
};
SIMPLE_ITEM(FireScroll);
SIMPLE_ITEM(Poem);
SIMPLE_ITEM(Corpse);
struct EventPoem {
  string SERIAL(eventName);
  SERIALIZE_ALL(eventName)
  ITEM_TYPE_INTERFACE;
};
struct Assembled {
  CreatureId SERIAL(creature);
  string SERIAL(itemName);
  EnumSet<MinionTrait> SERIAL(traits);
  optional<ItemUpgradeType> SERIAL(upgradeType);
  int SERIAL(maxUpgrades);
  SERIALIZE_ALL(NAMED(creature), NAMED(itemName), NAMED(traits), OPTION(upgradeType), OPTION(maxUpgrades))
  ITEM_TYPE_INTERFACE;
};
struct AutomatonPaint {
  Color SERIAL(color);
  SERIALIZE_ALL(color)
  ITEM_TYPE_INTERFACE;
};
using Simple = CustomItemId;
struct PrefixChance {
  double SERIAL(chance);
  HeapAllocated<ItemType> SERIAL(type);
  SERIALIZE_ALL(chance, type)
  ITEM_TYPE_INTERFACE;
};

#define ITEM_TYPES_LIST\
  X(Scroll, 0)\
  X(Potion, 1)\
  X(Mushroom, 2)\
  X(Amulet, 3)\
  X(Ring, 4)\
  X(TechBook, 5)\
  X(Intrinsic, 6)\
  X(Glyph, 7)\
  X(Simple, 8)\
  X(FireScroll, 9)\
  X(Poem, 10)\
  X(EventPoem, 11)\
  X(Assembled, 12) \
  X(Corpse, 13)\
  X(AutomatonPaint, 14)\
  X(PrefixChance, 15)\
  X(Balsam, 16)

#define VARIANT_TYPES_LIST ITEM_TYPES_LIST
#define VARIANT_NAME ItemTypeVariant

#include "gen_variant.h"

#undef VARIANT_TYPES_LIST
#undef VARIANT_NAME

template <class Archive>
void serialize(Archive& ar1, ItemTypeVariant& v);

}

class ItemTypeVariant : public ItemTypes::ItemTypeVariant {
  public:
  using ItemTypes::ItemTypeVariant::ItemTypeVariant;
};
