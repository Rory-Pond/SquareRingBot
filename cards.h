#pragma once
// #include "main.h"
#include "constants.h"

struct context
{
	int round_count = 0;
	std::string previous_card_name;
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
		bool counter = false;
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

class AttackCardWithEffect : public AttackCard
{
	private:
	public:
		AttackCardWithEffect(std::string name_, std::vector<int> attack_target_, int damage_, std::vector<std::string> attack_move_, bool cooldown_ = false, bool daze_ = false)
		: AttackCard(name_, attack_target_, damage_, attack_move_, cooldown_, daze_) {};
		virtual bool isEffectActive() const { return false; }
		virtual int conditionalDamage() const { return 0; }
		virtual bool conditionalDaze() const { return false; }

		int getDamage() const override {
			if (isEffectActive())
			{
				return damage + conditionalDamage();
			}
			else
			{
				return damage;
			}
		};

		bool daze() const override { 
			if (isEffectActive())
			{
				return conditionalDaze();
			}
			return attack_daze;
		};
};

class AttackCardWithRoundEffect : public AttackCardWithEffect
{
	private:
	public:
		std::array<int, 3> round_damage = {0, 0, 0};
		AttackCardWithRoundEffect(
			std::string name_,
			std::vector<int> attack_target_,
			int damage_,
			std::vector<std::string> attack_move_,
			bool cooldown_ = false,
			bool daze_ = false,
			std::array<int, 3> round_damage_ = {0, 0, 0} 
		) : AttackCardWithEffect(name_, attack_target_, damage_, attack_move_, cooldown_, daze_), round_damage(round_damage_) {};
		
		bool isEffectActive() const override
		{
			return true; 
		}
		bool conditionalDaze() const override
		{
			return attack_daze;
		}
		int conditionalDamage() const override
		{
			return 2;// round_damage[context.round_count];
		}
};

class AttackCardWithPreviousCardEffect : public AttackCardWithEffect
{
	private:
	public:
		std::string previous_card_name;
		int bonus_damage = 0;
		bool bonus_daze = false;

		AttackCardWithPreviousCardEffect(
			std::string name_, 
			std::vector<int> attack_target_, 
			int damage_, 
			std::vector<std::string> attack_move_, 
			bool cooldown_ = false, 
			bool daze_ = false, 
			std::string previous_card_name_ = "",
			int bonus_damage_ = 0,
			bool bonus_daze_ = false
		) : AttackCardWithEffect(name_, attack_target_, damage_, attack_move_, cooldown_, daze_), previous_card_name(previous_card_name_), bonus_damage(bonus_damage_), bonus_daze(bonus_daze_) {};
		
		bool isEffectActive() const override
		{
			return previous_card_name == "2";// context.previous_card_name;
		}
		bool conditionalDaze() const override
		{
			return bonus_daze;
		}
		int conditionalDamage() const override
		{
			return bonus_damage;
		}
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