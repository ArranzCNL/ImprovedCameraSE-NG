#include "RE/T/TESSpellList.h"

namespace RE
{
	TESSpellList::SpellData::SpellData() :
		spells(nullptr),
		levSpells(nullptr),
		shouts(nullptr),
		numSpells(0),
		numlevSpells(0),
		numShouts(0),
		pad24(0)
	{
	}

	void TESSpellList::SpellData::CopySpellList(const std::vector<TESLevSpell*>& a_copiedData)
	{
		const auto oldData = levSpells;

		const auto newSize = a_copiedData.size();
		const auto newData = calloc<TESLevSpell*>(newSize);
		std::ranges::copy(a_copiedData, newData);

		numlevSpells = static_cast<std::uint32_t>(newSize);
		levSpells = newData;

		free(oldData);
	}

	void TESSpellList::SpellData::CopySpellList(const std::vector<TESShout*>& a_copiedData)
	{
		const auto oldData = shouts;

		const auto newSize = a_copiedData.size();
		const auto newData = calloc<TESShout*>(newSize);
		std::ranges::copy(a_copiedData, newData);

		numShouts = static_cast<std::uint32_t>(newSize);
		shouts = newData;

		free(oldData);
	}

	void TESSpellList::SpellData::CopySpellList(const std::vector<SpellItem*>& a_copiedData)
	{
		const auto oldData = spells;

		const auto newSize = a_copiedData.size();
		const auto newData = calloc<SpellItem*>(newSize);
		std::ranges::copy(a_copiedData, newData);

		numSpells = static_cast<std::uint32_t>(newSize);
		spells = newData;

		free(oldData);
	}

	bool TESSpellList::SpellData::AddLevSpell(TESLevSpell* a_levSpell)
	{
		if (!GetIndex(a_levSpell)) {
			std::vector<TESLevSpell*> copiedData{ levSpells, levSpells + numlevSpells };
			copiedData.push_back(a_levSpell);
			CopySpellList(copiedData);
			return true;
		}
		return false;
	}

	bool TESSpellList::SpellData::AddLevSpells(const std::vector<TESLevSpell*>& a_levSpells)
	{
		std::vector<TESLevSpell*> copiedData{ levSpells, levSpells + numlevSpells };
		std::ranges::remove_copy_if(a_levSpells, std::back_inserter(copiedData), [&](auto& spell) {
			return std::ranges::find(copiedData, spell) != copiedData.end();
		});
		CopySpellList(copiedData);
		return true;
	}

	bool TESSpellList::SpellData::AddShout(TESShout* a_shout)
	{
		std::vector<TESShout*> copiedData{ shouts, shouts + numShouts };
		if (!GetIndex(a_shout)) {
			copiedData.push_back(a_shout);
			CopySpellList(copiedData);
			return true;
		}
		return false;
	}

	bool TESSpellList::SpellData::AddShouts(const std::vector<TESShout*>& a_shouts)
	{
		std::vector<TESShout*> copiedData{ shouts, shouts + numShouts };
		std::ranges::remove_copy_if(a_shouts, std::back_inserter(copiedData), [&](auto& shout) {
			return std::ranges::find(copiedData, shout) != copiedData.end();
		});
		CopySpellList(copiedData);
		return true;
	}

	bool TESSpellList::SpellData::AddSpell(SpellItem* a_spell)
	{
		std::vector<SpellItem*> copiedData{ spells, spells + numSpells };
		if (!GetIndex(a_spell)) {
			copiedData.push_back(a_spell);
			CopySpellList(copiedData);
			return true;
		}
		return false;
	}

	bool TESSpellList::SpellData::AddSpells(const std::vector<SpellItem*>& a_spells)
	{
		std::vector<SpellItem*> copiedData{ spells, spells + numSpells };
		std::ranges::remove_copy_if(a_spells, std::back_inserter(copiedData), [&](auto& spell) {
			return std::ranges::find(copiedData, spell) != copiedData.end();
		});
		CopySpellList(copiedData);
		return true;
	}

	std::optional<std::uint32_t> TESSpellList::SpellData::GetIndex(const SpellItem* a_spell) const
	{
		if (spells) {
			for (std::uint32_t i = 0; i < numSpells; i++) {
				if (spells[i] == a_spell) {
					return i;
				}
			}
		}
		return std::nullopt;
	}

	std::optional<std::uint32_t> TESSpellList::SpellData::GetIndex(const TESLevSpell* a_levSpell) const
	{
		if (levSpells) {
			for (std::uint32_t i = 0; i < numlevSpells; i++) {
				if (levSpells[i] == a_levSpell) {
					return i;
				}
			}
		}
		return std::nullopt;
	}

	std::optional<std::uint32_t> TESSpellList::SpellData::GetIndex(const TESShout* a_shout) const
	{
		if (shouts) {
			for (std::uint32_t i = 0; i < numShouts; i++) {
				if (shouts[i] == a_shout) {
					return i;
				}
			}
		}
		return std::nullopt;
	}

	bool TESSpellList::SpellData::RemoveLevSpell(TESLevSpell* a_levSpell)
	{
		std::vector<TESLevSpell*> copiedData{ levSpells, levSpells + numlevSpells };
		if (std::erase(copiedData, a_levSpell) > 0) {
			CopySpellList(copiedData);
			return true;
		}
		return false;
	}

	bool TESSpellList::SpellData::RemoveLevSpells(const std::vector<TESLevSpell*>& a_levSpells)
	{
		std::vector<TESLevSpell*> copiedData{ levSpells, levSpells + numlevSpells };
		if (std::erase_if(copiedData, [&](auto& spell) { return std::ranges::find(a_levSpells, spell) != a_levSpells.end(); }) > 0) {
			CopySpellList(copiedData);
			return true;
		}
		return false;
	}

	bool TESSpellList::SpellData::RemoveShout(TESShout* a_shout)
	{
		std::vector<TESShout*> copiedData{ shouts, shouts + numShouts };
		if (std::erase(copiedData, a_shout) > 0) {
			CopySpellList(copiedData);
			return true;
		}
		return false;
	}

	bool TESSpellList::SpellData::RemoveShouts(const std::vector<TESShout*>& a_shouts)
	{
		std::vector<TESShout*> copiedData{ shouts, shouts + numShouts };
		if (std::erase_if(copiedData, [&](auto& shout) { return std::ranges::find(a_shouts, shout) != a_shouts.end(); }) > 0) {
			CopySpellList(copiedData);
			return true;
		}
		return false;
	}

	bool TESSpellList::SpellData::RemoveSpell(SpellItem* a_spell)
	{
		std::vector<SpellItem*> copiedData{ spells, spells + numSpells };
		if (std::erase(copiedData, a_spell) > 0) {
			CopySpellList(copiedData);
			return true;
		}
		return false;
	}

	bool TESSpellList::SpellData::RemoveSpells(const std::vector<SpellItem*>& a_spells)
	{
		std::vector<SpellItem*> copiedData{ spells, spells + numSpells };
		if (std::erase_if(copiedData, [&](auto& spell) { return std::ranges::find(a_spells, spell) != a_spells.end(); }) > 0) {
			CopySpellList(copiedData);
			return true;
		}
		return false;
	}
}
