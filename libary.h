#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <chrono>

#include "main.h"
#include "cards.h"

class Libary
{
	private:
	const std::vector<int> Attack_Forwards = {1};
	const std::vector<int> Attack_Left = {8, 1};
	const std::vector<int> Attack_Right = {1, 2};
	const std::vector<int> Attack_All = {8, 1, 2};
	const std::vector<int> Attack_Far_Left = {10, 11};
	const std::vector<int> Attack_Far_Right = {9, 10};
	const std::vector<int> Attack_Far_All = {9, 10, 11};
	const std::vector<int> Attack_Double_Forwards = {1, 10};
	const std::vector<int> Attack_Double_Left = {2, 12};
	
	const bool HAS_COOLDOWN = true;
	const bool HAS_DAZE = true;
	const bool HAS_COUNTER = true;
	const bool increase_Burst = true;
	const bool CannotBeDazed = true;
	const bool CanBeDazed = false;
	
	std::unordered_map<std::string, std::unique_ptr<BaseCard>> cards;
	
	public:
	Libary()
	{	
		cards["Null"] = std::make_unique<BaseCard>("Null");
		cards["Rotate_45"] = std::make_unique<MoveCard>("Rotate_45", 1, 0);
		cards["Rotate_90"] = std::make_unique<MoveCard>("Rotate_90", 2, 0);
		cards["Move_Forwards"] = std::make_unique<MoveCard>("Move_Forwards", 0, 1);
		cards["Move_Forwards_Rotate"] = std::make_unique<MoveCard>("Move_Forwards_rotate", 1, 1);
		cards["Move_Forwards_Right"] = std::make_unique<MoveCard>("Move_Forwards_Right", 0, 2);
		cards["Move_Forwards_Right_Rotate"] = std::make_unique<MoveCard>("Move_Forwards_Right_rotate", 1, 2);
		cards["Move_Right"] = std::make_unique<MoveCard>("Move_Right", 0, 3);
		cards["Move_Right_Rotate"] = std::make_unique<MoveCard>("Move_Right_rotate", 1, 3);
		cards["Move_Backwards_Right"] = std::make_unique<MoveCard>("Move_Backwards_Right", 0, 4);
		cards["Move_Backwards_Right_Rotate"] = std::make_unique<MoveCard>("Move_Backwards_Right_rotate", 1, 4);
		cards["Move_Backwards"] = std::make_unique<MoveCard>("Move_Backwards", 0, 5);
		cards["Move_Backwards_Rotate"] = std::make_unique<MoveCard>("Move_Backwards_rotate", 1, 5);
		cards["Move_Backwards_Left"] = std::make_unique<MoveCard>("Move_Backwards_Left", 0, 6);
		cards["Move_Backwards_Left_Rotate"] = std::make_unique<MoveCard>("Move_Backwards_Left_rotate", 1, 6);
		cards["Move_Left"] = std::make_unique<MoveCard>("Move_Left", 0, 7);
		cards["Move_Left_Rotate"] = std::make_unique<MoveCard>("Move_Left_rotate", 1, 7);
		cards["Move_Forwards_Left"] = std::make_unique<MoveCard>("Move_Forwards_Left", 0, 8);
		cards["Move_Forwards_Left_Rotate"] = std::make_unique<MoveCard>("Move_Forwards_Left_rotate", 1, 8);
		cards["Token_None"] = std::make_unique<TokenCard>("Token_None", TokenType::None);
		cards["Token_Burst"] = std::make_unique<TokenCard>("Token_Burst", TokenType::Burst);
		cards["Token_React"] = std::make_unique<TokenCard>("Token_React", TokenType::React);
		// std::array<int, 3> round_buff = {0, -1, -2};
		// cards["Cannonita_Oscar"] = std::make_unique<AttackCardWithRoundEffect>("Cannonita_Oscar", Attack_Double_Forwards, 5, {}, HAS_COOLDOWN, false, round_buff);
		// cards["Cajones_Oscar"] = std::make_unique<DamageModificationCard>("Cajones_Oscar", 3, CanBeDazed, increase_Burst); //attack move rotate
		// cards["Left_Haymaker_Oscar"] = std::make_unique<AttackCardWithRoundEffect>("Left_Haymaker_Oscar", Attack_Left,4, {}, false, false, {0, -1, -2});
		// cards["Left_Haymaker_Oscar"] = std::make_unique<AttackCardWithRoundEffect>("El_Nuke_Oscar", Attack_Left, 8, {}, HAS_COOLDOWN, HAS_DAZE, {0, -2, -4});

		// cards["Left_Jab_Inga"] = std::make_unique<AttackCard>("Left_Jab_Inga", Attack_Far_Left, 1, std::vector<std::string>{"Move_Left", "Move_Forwards_Left_Rotate", "Move_Backwards_Left_Rotate"});
		// cards["Right_Hook_Inga"] = std::make_unique<AttackCard>("Right_Hook_Inga", Attack_Right, 2, std::vector<std::string>{"Move_Right"});
		// cards["Svallin_Inga"] = std::make_unique<DamageModificationCard>("Svallin_Inga", -3, CannotBeDazed);
		// cards["Mjolnir_Inga"] = std::make_unique<AttackCardWithRoundEffect>("Mjolnir_Inga", Attack_Double_Forwards, 4, {}, HAS_COOLDOWN, HAS_DAZE, {0, 1, 2});
		
		// cards["Left_Hook_Fredo"] = std::make_unique<AttackCardWithPreviousCardEffect>("Left_Hook_Fredo", Attack_Left, 3, {}, false, false, "Grill_Guard_Fredo", 1);
		// cards["Right_Hook_Fredo"] = std::make_unique<PreviousCardEffect>("Right_Hook_Fredo", Attack_Right, 3, {}, false, false, "Grill_Guard_Fredo", 1);
		// cards["Uppercut_Fredo"] = std::make_unique<PreviousCardEffect>("Uppercut_Fredo", Attack_Forwards, 5, {}, false, false, "Grill_Guard_Fredo", 3, true);
		// // cards["Grill_Guard_Fredo"] = std::make_unique<AttackCard>("Grill_Guard_Fredo", "Rotate_45"); //todo damage reduction -2 prevent daze
		
		// cards["Collider_Andrey"] = std::make_unique<PreviousCardEffect>("Collider_Andrey", Attack_Double_Forwards, 2, "Quantum Jab", +2, std::vector<std::string>{"Move_Right_Rotate"});
		// // cards["Quantum_Jab_Andrey"] = std::make_unique<AttackCard>("Quantum_Jab_Andrey", Attack_Far_All, 1, "Move_Left_Rotate", "Move_Right_Rotate", HAS_COUNTER);
		// // cards["Paradox_Andrey"] = std::make_unique<AttackCard>("Paradox_Andrey");  //todo swap for collider or left_rotate
		// // cards["Entangle_Andrey"] = std::make_unique<AttackCard>("Entangle_Andrey", Attack_Forwards, 0); //todo if lands, no damage dealt by either character till movement
		
		// // cards["Left_Jab_Ellie"] = std::make_unique<AttackCard>("Left_Jab_Ellie", Attack_Far_Left, 1, std::vector<std::string>{"Move_Forwards_Rotate", "Move_Forwards_Right_Rotate", "Move_Right_Rotate", "Move_Backwards_Right_Rotate", "Move_Backwards_Rotate", "Move_Backwards_Left_Rotate", "Move_Left_Rotate", "Move_Forwards_Left_Rotate"});
		// // cards["Godiva_Strafe_Ellie"] = std::make_unique<AttackCard>("Godiva_Strafe_Ellie", Attack_Double_Left, 2, "Move_Forwards", "Move_Left", "Move_Backwards");//todo counter if react count == 2
		// // cards["Groove_Ellie"] = std::make_unique<AttackCard>("Groove_Ellie");//todo if you don’t get hit +1 burst or react; double damage from opponent if hit
		// // cards["Kook_Hook_Ellie"] = std::make_unique<AttackCard>("Kook_Hook_Ellie", Attack_Left, 3, std::vector<std::string>{"Move_Left"});//todo +1 damage if you have 2 bursts
		
		// // cards["Bolo_Punch_Awan"] = std::make_unique<AttackCard>("Bolo_Punch_Awan", Attack_Forwards, 4, HAS_DAZE);  //todo +1 damage and counter if in exchange 5/6
		// // cards["Flurry_Awan"] = std::make_unique<AttackCard>("Flurry_Awan", Attack_All, 1, std::vector<std::string>{"Move_Right_Rotate"});  //todo +1 damage and counter if in exchange 5/6
		// // cards["Left_Jab_Awan"] = std::make_unique<AttackCard>("Left_Jab_Awan", Attack_Far_Left, 1, std::vector<std::string>{"Move_Forwards_Left_Rotate", "Move_Right_Rotate"});  //todo +1 damage and counter if in exchange 5/6
		// // cards["Rush_Guard_Awan"] = std::make_unique<AttackCard>("Rush_Guard_Awan", HAS_COOLDOWN, "Move_Forwards_Rotate");  //damage reduction  -1, Cannot be dazed
		
		// // cards["Check_Hook_Roderik"] = std::make_unique<AttackCard>("Check_Hook_Roderik", Attack_Left, 1, HAS_COOLDOWN, std::vector<std::string>{"Move_Left"}); //todo (+2 if counter) 
		// // cards["Cobra_Cross_Roderik"] = std::make_unique<AttackCard>("Cobra_Cross_Roderik", Attack_Double_Forwards, 1, HAS_COOLDOWN, HAS_COUNTER); //todo (+1 if counter)
		// // cards["Left_Jab_Roderik"] = std::make_unique<AttackCard>("Left_Jab_Roderik", Attack_Far_Left, 1, HAS_COUNTER, std::vector<std::string>{"Move_Left_Rotate", "Move_Right", "Move_Backwards_Rotate"}); {"Left_Jab_Roderik", std::make_unique<AttackCard>("Left_Jab_Roderik", Attack_Far_Left, 1, HAS_COUNTER, {"Move_Left_Rotate", "Move_Right", "Move_Backwards_Rotate"}  ) },
		// // cards["Coil_Up_Roderik"] = std::make_unique<AttackCard>("Coil_Up_Roderik", HAS_COOLDOWN); //todo prevent all damage; if attack would land, next attack gets +2 damage and daze
		
		// // cards["Straight_Nigel"] = std::make_unique<AttackCard>("Straight_Nigel", Attack_Double_Forwards, 3, std::vector<std::string>{"Move_Forwards", "Move_Backwards"}); {"Straight_Nigel", std::make_unique<AttackCard>("Straight_Nigel", Attack_Double_Forwards, 3, {"Move_Forwards", "Move_Backwards"}  ) },
		// // cards["Wallop_Nigel"] = std::make_unique<AttackCard>("Wallop_Nigel", Attack_Left, 3, HAS_COOLDOWN);  //todo if hits, opponent moves backwards 1
		// // cards["Sway_Nigel"] = std::make_unique<AttackCard>("Sway_Nigel", HAS_COOLDOWN); //dodge; next action must be attack or backwards_rotate
		// // cards["Right_Jolt_Nigel"] = std::make_unique<AttackCard>("Right_Jolt_Nigel", Attack_Far_Right, 2, std::vector<std::string>{"Move_Forwards_Left"});
		
	}
	
	BaseCard* getCard(const std::string& name) const
	{
		auto it = cards.find(name);
		if (it != cards.end())
		{
			return it->second.get();
		}
		else
		{
			std::cerr << "Card not found: " << name << std::endl;
			throw std::runtime_error("Card not found");
			return nullptr;
		}
	}
	std::vector<BaseCard*> getCard(const std::vector<std::string>& name) const
	{
		std::vector<BaseCard*> result;
		for (const auto& card_name : name)
		{
			result.push_back(getCard(card_name));
		}
		return result;
	}
	std::vector<std::vector<BaseCard*>> getCard(const std::vector<std::vector<std::string>>& name) const
	{
		std::vector<std::vector<BaseCard*>> result;
		for (const auto& card_names : name)
		{
			result.push_back(getCard(card_names));
		}
		return result;
	}
};