#pragma once

#include <stdint.h>

#include <array>
#include <memory>
#include <phosg/JSON.hh>
#include <random>
#include <string>

#include "AFSArchive.hh"
#include "GSLArchive.hh"
#include "ItemNameIndex.hh"
#include "StaticGameData.hh"
#include "Text.hh"
#include "Version.hh"

class RareItemSet {
public:
  struct ExpandedDrop {
    uint32_t probability = 0;
    parray<uint8_t, 3> item_code;

    std::string str() const;
    std::string str(Version version, std::shared_ptr<const ItemNameIndex> name_index) const;
  };

  RareItemSet();
  RareItemSet(const AFSArchive& afs, bool is_v1);
  RareItemSet(const GSLArchive& gsl, bool is_big_endian);
  RareItemSet(const std::string& rel, bool is_big_endian);
  RareItemSet(const JSON& json, Version version, std::shared_ptr<const ItemNameIndex> name_index = nullptr);
  ~RareItemSet() = default;

  std::vector<ExpandedDrop> get_enemy_specs(GameMode mode, Episode episode, uint8_t difficulty, uint8_t secid, uint8_t rt_index) const;
  std::vector<ExpandedDrop> get_box_specs(GameMode mode, Episode episode, uint8_t difficulty, uint8_t secid, uint8_t area) const;

  std::string serialize_afs() const;
  std::string serialize_gsl(bool big_endian) const;
  std::string serialize_json(Version version, std::shared_ptr<const ItemNameIndex> name_index = nullptr) const;

  void print_collection(
      FILE* stream,
      Version version,
      GameMode mode,
      Episode episode,
      uint8_t difficulty,
      uint8_t section_id,
      std::shared_ptr<const ItemNameIndex> name_index = nullptr) const;
  void print_all_collections(FILE* stream, Version version, std::shared_ptr<const ItemNameIndex> name_index = nullptr) const;

protected:
  struct SpecCollection {
    std::vector<std::vector<ExpandedDrop>> rt_index_to_specs;
    std::vector<std::vector<ExpandedDrop>> box_area_to_specs;
  };

  struct ParsedRELData {
    struct PackedDrop {
      uint8_t probability = 0;
      parray<uint8_t, 3> item_code;

      PackedDrop() = default;
      explicit PackedDrop(const ExpandedDrop&);
      ExpandedDrop expand() const;
    } __attribute__((packed));

    template <bool IsBigEndian>
    struct Offsets {
      using U32T = typename std::conditional<IsBigEndian, be_uint32_t, le_uint32_t>::type;
      /* 00 */ U32T monster_rares_offset; // -> parray<PackedDrop, 0x65> (or 0x33 on v1)
      /* 04 */ U32T box_count; // Usually 30 (0x1E)
      /* 08 */ U32T box_areas_offset; // -> parray<uint8_t, 0x1E>
      /* 0C */ U32T box_rares_offset; // -> parray<PackedDrop, 0x1E>
      /* 10 */
    } __attribute__((packed));

    struct BoxRare {
      uint8_t area;
      ExpandedDrop drop;
    };

    std::vector<ExpandedDrop> monster_rares;
    std::vector<BoxRare> box_rares;

    ParsedRELData() = default;
    ParsedRELData(StringReader r, bool big_endian, bool is_v1);
    explicit ParsedRELData(const SpecCollection& collection);
    std::string serialize(bool big_endian) const;

    template <bool IsBigEndian>
    void parse_t(StringReader r, bool is_v1);
    template <bool IsBigEndian>
    std::string serialize_t() const;

    SpecCollection as_collection() const;
  };

  std::unordered_map<uint16_t, SpecCollection> collections;

  const SpecCollection& get_collection(GameMode mode, Episode episode, uint8_t difficulty, uint8_t secid) const;

  static std::string gsl_entry_name_for_table(GameMode mode, Episode episode, uint8_t difficulty, uint8_t section_id);
  static uint16_t key_for_params(GameMode mode, Episode episode, uint8_t difficulty, uint8_t secid);

  static uint32_t expand_rate(uint8_t pc);
  static uint8_t compress_rate(uint32_t probability);
};
