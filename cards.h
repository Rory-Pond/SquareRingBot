#pragma once
// #include "main.h"
#include "constants.h"

struct context
{
	/* data */
};

class BaseCard
{
	protected:
		std::string name;
	public:
		
		virtual std::string getName() const {return name;};
		virtual int getMove() const {return 0;}
		virtual int getRotate() const { return 0; }

		virtual int getDamage() const { return 0; }
		virtual std::vector<int> getAttackTarget() const { return {}; }
		virtual bool attack_is_counter() const { return false; }
		virtual bool daze() const { return false; }
		virtual std::vector<std::string> getAttackMove() const { return {"Null"}; }
		virtual bool isCooldown() const { return false; }
		
		virtual bool avoids_daze() const { return false; }
		virtual int getDamageModification(int damage) const { return damage; }
		virtual bool dodges_all_attacks() const { return false; }
		virtual TokenType getToken() const { return TokenType::Not_Submitted; }

		virtual int addBurst() const { return 0; }
		virtual int addReact() const { return 0; }
		
		virtual ~BaseCard() = default;
		BaseCard(std::string name_)
		: name(name_) {};
};

class MoveCard : public BaseCard
{
	private:
	public:
		MoveCard(std::string name_, int rotate_, int move_)
		: BaseCard(name_), rotate(rotate_), move(move_) {};
		
		int rotate = 0;
		int move = 0;
		int getMove() const override {return move;};
		int getRotate() const override {return rotate;};
};

class TokenCard : public BaseCard
{
	private:
	public:
		TokenType token = TokenType::Not_Submitted;
		TokenCard(std::string name_, TokenType token_)
		: BaseCard(name_), token(token_) {};
		
		TokenType getToken() const override {return token;};
};

class AttackCard : public BaseCard
{
	private:
	public:
		std::vector<int> attack_target;
		int damage = 0;
		// bool counter = false;
		bool cooldown = false;
		bool attack_daze = false;
		std::vector<std::string> attack_move;
		
		AttackCard(std::string name_, std::vector<int> attack_target_, int damage_, std::vector<std::string> attack_move_, bool cooldown_ = false, bool daze_ = false)
		: BaseCard(name_), attack_target(attack_target_), damage(damage_), attack_move(attack_move_), cooldown(cooldown_), attack_daze(daze_) {};

		int getDamage() const override {return damage;};
		std::vector<int> getAttackTarget() const override {return attack_target;};
		// bool attack_is_counter() const override {return attack_is_counter;};
		bool isCooldown() const override {return cooldown;};
		std::vector<std::string> getAttackMove() const override {return attack_move;};
		bool daze() const override { return attack_daze; };
};

class DamageModificationCard : public BaseCard
{
	private:
	public:
		int damageModifier = 0;
		bool CannotBeDazed = false;
		bool increase_Burst_ = false;

		DamageModificationCard(std::string name_, int damageModifier_ = 0, bool CannotBeDazed_ = false, bool increase_Burst_ = false)
		: BaseCard(name_), damageModifier(damageModifier_), CannotBeDazed(CannotBeDazed_), increase_Burst_(increase_Burst_) {};

		int getDamageModification(int damage) const override
		{
			return damage + damageModifier;
		}
		bool avoids_daze() const override {return CannotBeDazed;};
		int addBurst() const override
		{
			if(increase_Burst_)
			{
				return 1;
			}
			return 0;
		}
};

class AttackRoundCard : public BaseCard
{
	private:
	public:
		int round_1_damage = 0;
		int round_2_damage = 0;	
		int round_3_damage = 0;
		std::vector<int> attack_target;
		bool cooldown = false;
		bool attack_daze = false;
		AttackRoundCard(std::string name_, std::vector<int> attack_target_ , int damage_1, int damage_2, int damage_3, bool cooldown = false, bool attack_daze = false)
		: BaseCard(name_), attack_target(attack_target_), round_1_damage(damage_1), round_2_damage(damage_2), round_3_damage(damage_3), cooldown(cooldown), attack_daze(attack_daze) {};
		
		int getDamage() const override
		{
			switch (1) //(context.round_count)
			{
				case 1:
					return round_1_damage;
				case 2:
					return round_2_damage;
				case 3:
					return round_3_damage;
				default:
					return 0;
			}
		}
		std::vector<int> getAttackTarget() const override {return attack_target;};
		// bool attack_is_counter() const override {return attack_is_counter;};
		bool isCooldown() const override {return cooldown;};
		bool daze() const override {return attack_daze;};
};

// {
// 	private:
// 	public:
// 		std::string name;
// 		TokenType token = TokenType::Not_Submitted;
// 		int rotate = 0;
// 		int move = 0;
// 		int damage = 0, damage_reduction = 0, attack_damage = 0;
// 		bool attack_is_dodge = false, attack_is_counter =false;
// 		bool cooldown = false;
// 		bool daze = false;
// 		std::vector<std::string> attack_move = {"Null"};
// 		std::vector<int> attack_target;
// 		BaseCard(std::string name_, int rotate_, int move_)
// 		: name(name_), rotate(rotate_), move(move_) {};
		
// 		BaseCard(std::string name_, TokenType token_) 
// 		: token(token_) {};
		
// 		BaseCard(std::string name_)
// 		: name(name_) {};

// 		virtual std::string getName() {return name;};
// 		virtual int getMove() {return move;};
// 		virtual int getRotate() {return rotate;};
// 		virtual int getDamage() {return damage;};
// 		virtual int getDamageReduction() {return damage_reduction;};
// 		virtual bool isAttackDodge() {return attack_is_dodge;};
// 		virtual bool isAttackCounter() {return attack_is_counter;};
// 		virtual bool isCooldown() {return cooldown;};
// 		virtual bool isDaze() {return daze;};
// 		virtual TokenType getToken() {return token;};
// 		virtual std::vector<int> getAttackTarget() {return attack_target;};
// 		virtual std::vector<BaseCard*> getAttackMove() {return {};};
// 		virtual int addBurst() {return 0;};
// 		virtual int addReact() {return 0;};

// };


// class AttCard : public BaseCard
// {
// 	private:
// 	public:
// 		AttCard(std::string name_,  std::vector<int> attack_target_ , int damage_, bool cooldown = false, bool daze = false)
// 		: BaseCard(name_), attack_target(attack_target_), damage(damage_) {};
		
// };

// class DamageReductionCard : public BaseCard
// {
// 	private:
// 	public:
// 		int damageModifier = 0;
// 		bool increase_Burst = false;
// 		bool increase_React = false;

// 		BaseCard(std::string name_, int rotate_, int move_, int damage_reduction, bool increase_Burst = false, bool increase_React = false)
// 		: name(name_), rotate(rotate_), move(move_), damageModifier(damageModifier_), increase_Burst(increase_Burst), increase_React(increase_React) {};
// 		get_incerease_Burst() {return increase_Burst;};
// 		get_incerease_React() {return increase_React;};
// 		int getDamage() const override
// 		{
// 			return damage + damageModifier;
// 		}
// };

// class RoundCard : public BaseCard
// {
// 	private:
// 	public:
// 		int round_1_damage = 0;
// 		int round_2_damage = 0;	
// 		int round_3_damage = 0;
// 		RoundCard(std::string name_,  std::vector<int> attack_target_ , int damage_1, int damage_2, int damage_3, bool cooldown = false, bool daze = false)
// 		: name(name_), attack_target(attack_target_), round_1_damage(damage_1), round_2_damage(damage_2), round_3_damage(damage_3), cooldown(cooldown), daze(daze) {};
// 		int getDamage() const override
// 		{
// 			switch (context.round_count)
// 			{
// 				case 1:
// 					return round_1_damage;
// 				case 2:
// 					return round_2_damage;
// 				case 3:
// 					return round_3_damage;
// 				default:
// 					return 0;
// 			}
// 		}
// };


// class PreviousAtctionCard : public BaseCard
// {
// 	private:
// 	public:
// 		std::string previous_card_name;
// 		int bonus_damage = 0;
// 		bool bonus_daze = false;
// 		PreviousAtctionCard(std::string name_,  std::vector<int> attack_target_ , int damage_, std::string previous_card_name_, int bonus_damage_ = 0, bool bonus_daze_ = false)
// 		: name(name_), attack_target(attack_target_), damage(damage_), previous_card_name(previous_card_name_), bonus_damage(bonus_damage_), bonus_daze(bonus_daze_) {};
		
// 		bool isEffectActive()
// 		{
// 			return context.previousPlayedCardName == previous_card_name;
// 		}
// 		int getDamage() const override
// 		{
// 			if (isEffectActive())
// 			{
// 				return damage + bonus_damage;
// 			}
// 			else
// 			{
// 				return damage;
// 			}
// 		}
// 		bool isDaze() const override
// 		{
// 			if (isEffectActive())
// 			{
// 				return bonus_daze;
// 			}
// 			else
// 			{
// 				return daze;
// 			}
// 		}
// };
