#pragma once

#include <vector>
#include <string>
#include "Constants.h"

struct context
{
	int round_count = 0;
	std::string previous_card_name;
};

class BaseCard
{
	public:
		std::string name;

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
		BaseCard(std::string name)
		: name(name) {};
};

class MoveCard : public BaseCard
{
	private:
	public:
		MoveCard(std::string name, int rotate, int move)
		: BaseCard(name), rotate(rotate), move(move) {};

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
		TokenCard(std::string name, TokenType token)
		: BaseCard(name), token(token) {};

		TokenType getToken() const override {return token;};
};

class AttackCard : public BaseCard
{
	private:
	public:
		AttackCard(
			std::string name,
			std::vector<int> attack_target,
			int damage,
			std::vector<std::string> attack_move,
			bool cooldown = false,
			bool daze = false,
			bool counter = false
		) : BaseCard(name), attack_target(attack_target), damage(damage), attack_move(attack_move), cooldown(cooldown), attack_daze(daze), counter(counter) {};

		std::vector<int> attack_target;
		int damage = 0;
		bool counter = false;
		bool cooldown = false;
		bool attack_daze = false;
		std::vector<std::string> attack_move;

		int getDamage() const override {return damage;};
		std::vector<int> getAttackTarget() const override {return attack_target;};
		bool attack_is_counter() const override {return counter;};
		bool isCooldown() const override {return cooldown;};
		std::vector<std::string> getAttackMove() const override {return attack_move;};
		bool daze() const override { return attack_daze; };
};

class AttackCardWithEffect : public AttackCard
{
	private:
	public:
		AttackCardWithEffect(
			std::string name,
			std::vector<int> attack_target,
			int damage,
			std::vector<std::string> attack_move,
			bool cooldown = false,
			bool daze = false,
			bool counter = false
		) : AttackCard(name, attack_target, damage, attack_move, cooldown, daze, counter) {};

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
			std::string name,
			std::vector<int> attack_target,
			int damage,
			std::vector<std::string> attack_move,
			bool cooldown = false,
			bool daze = false,
			bool counter = false,
			std::array<int, 3> round_damage = {0, 0, 0}
		) : AttackCardWithEffect(name, attack_target, damage, attack_move, cooldown, daze, counter), round_damage(round_damage) {};

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
			std::string name,
			std::vector<int> attack_target,
			int damage,
			std::vector<std::string> attack_move,
			bool cooldown = false,
			bool daze = false,
			bool counter = false,
			std::string previous_card_name = "",
			int bonus_damage = 0,
			bool bonus_daze = false
		) : AttackCardWithEffect(name, attack_target, damage, attack_move, cooldown, daze, counter), previous_card_name(previous_card_name), bonus_damage(bonus_damage), bonus_daze(bonus_daze) {};

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

		DamageModificationCard(
			std::string name,
			int damageModifier_ = 0,
			bool CannotBeDazed_ = false,
			bool increase_Burst_ = false
		) : BaseCard(name), damageModifier(damageModifier_), CannotBeDazed(CannotBeDazed_), increase_Burst_(increase_Burst_) {};

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