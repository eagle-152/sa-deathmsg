#include "plugin.h"
#include "CPlayerInfo.h"
#include "CWorld.h"
#include "CDarkel.h"
#include "CMessages.h"
#include "CStats.h"
#include "CGangWars.h"
#include "CText.h"

using namespace plugin;

class DeathMessages {
public:
    DeathMessages() {
		Events::initGameEvent += [] {
			patch::RedirectJump(0x56E580, CPlayerInfoKillPlayer);
		};
    }

	static void CPlayerInfoKillPlayer() {
		CPlayerInfo *info = &CWorld::Players[CWorld::PlayerInFocus];

		if (!info) {
			return;
		}
			
		if (info->m_nPlayerState == PLAYERSTATE_PLAYING) {
			info->m_nPlayerState = PLAYERSTATE_HASDIED;
			CDarkel::ResetOnPlayerDeath();

			CPlayerPed *player = FindPlayerPed();

			if (player && player->m_pFire) {
				CMessages::AddBigMessage(TheText.Get("FRIED"), 4000, 2);
			}
			else {
				switch (info->m_pPed->m_nLastWeaponDamage) {
				case 49: // Ram
				case 50: // Runover
					CMessages::AddBigMessage(TheText.Get("PANCAKED"), 4000, 2);
					break;

				case 53: // Drown
					CMessages::AddBigMessage(TheText.Get("SOAKED"), 4000, 2);
					break;

				case 54: // Fall
					CMessages::AddBigMessage(TheText.Get("SPLAT"), 4000, 2);
					break;

				case 51: // Explosion
				case 59: // Tank grenade?
					CMessages::AddBigMessage(TheText.Get("KABOOM"), 4000, 2);
					break;

				default:
					CMessages::AddBigMessage(TheText.Get("DEAD"), 4000, 2);
				}
			}

			CStats::IncrementStat(135, 1.0);
			CGangWars::EndGangWar(false);
		}
	}
} deathMessages;