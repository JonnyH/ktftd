/*
 * =====================================================================================
 *
 *       Filename:  datafiles.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  25/09/12 09:37:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __DATAFILES_H
#define __DATAFILES_H

#include <cstdint>

#define offsetof(st, m) __builtin_offsetof(st, m)

namespace ktftd
{
namespace datafiles
{

	class vec2_int16
	{
	public:
		int16_t x;
		int16_t y;
	};

	class acts_dat
	{
	public:
		class
		{
		public:
			uint8_t research;
			uint8_t harvest;
			uint8_t abduction;
			uint8_t infiltration;
			uint8_t base;
			uint8_t terror;
			uint8_t retaliation;
		} zonePriorities[12];
	};
	static_assert(sizeof(acts_dat) == 84, "Expected acts_dat to be 84 bytes");

	class activity_dat
	{
		public:
		class
		{
			public:
			class 
			{
			public:
				int32_t USA;
				int32_t Russia;
				int32_t UK;
				int32_t France;
				int32_t Germany;
				int32_t Italy;
				int32_t Spain;
				int32_t China;
				int32_t Japan;
				int32_t India;
				int32_t Brazil;
				int32_t Australia;
				int32_t Nigeria;
				int32_t SouthAfrica;
				int32_t Egypt;
				int32_t Canada;
			} countries;
			class
			{
			public:
				int32_t NorthAmeria;
				int32_t Arctic;
				int32_t Antartica;
				int32_t SouthAmeria;
				int32_t Europe;
				int32_t NorthAfrica;
				int32_t SouthernAfrica;
				int32_t CentralAsia;
				int32_t SoutheastAsia;
				int32_t Siberia;
				int32_t Australasia;
				int32_t Pacific;
				int32_t NorthAtlantic;
				int32_t SouthAtlantic;
				int32_t IndianOcean;
			}areas;
		} months[12];
	};

	static_assert(sizeof(activity_dat) == 1488, "Expected activity_dat to be 1488 bytes");
	//xcom.dat and alien.dat are the same format
	using xcom_dat = activity_dat;
	using alien_dat = activity_dat;


	class astore_dat_entry
	{
	public:
		uint8_t type;
		uint8_t rank;
		uint8_t containingBase;
		uint8_t unknown[9];
	};
	static_assert(sizeof(astore_dat_entry) == 12, "Expected astore_dat_entry to be 12 bytes");

	class base_dat_entry
	{
	public:
		uint8_t name[15];
		uint8_t unknown1;//Null terminating string?
		class
		{
		public:
			int16_t items[98];
		} stores;
		uint16_t shortDetection;
		uint16_t longDetection;
		uint16_t resolverDetection;
		uint8_t facilities[6][6];
		uint8_t daysToBuild[6][6];
		uint8_t technicians;
		uint8_t scientists;
		uint8_t disabled;
		uint8_t unknown2[3];
	};
	static_assert(sizeof(base_dat_entry) == 296, "Expected base_dat_entry to be 296 bytes");

	static const int MANUFACTURABLE_ITEM_COUNT = 43;

	class bprod_dat
	{
	public:
		class
		{
		public:
			int32_t hoursStored[MANUFACTURABLE_ITEM_COUNT];//There seem to be 43 producable items in TFTD
			int16_t allocatedEngineers[MANUFACTURABLE_ITEM_COUNT];
			int16_t totalToProduce[MANUFACTURABLE_ITEM_COUNT];
			int16_t unitsProduced[MANUFACTURABLE_ITEM_COUNT];
		}__attribute__((packed))base[8];
	};
	static_assert(sizeof(bprod_dat) == 3440, "Expected bprod_dat to be 3440 bytes");

	class craft_dat
	{
	public:
		class
		{
		public:
			uint8_t type;
			uint8_t leftWeaponType;
			int16_t leftWeaponAmmo;
			uint8_t action;
			uint8_t rightWeaponType;
			uint8_t inventory[55];
			uint8_t unknown1;
			int16_t rightWeaponAmmo;
			uint8_t unknown2[2];
			int16_t damage;
			int16_t depth;
			int16_t speed;
			uint16_t destinationLocIdx;
			int16_t airborneTracking;//-1 is airborne (ignoring depth), 0 is tracking but not attacking
			vec2_int16 destinationCoords;
			int16_t fuel;
			uint16_t baseLocationIdx;
			uint16_t mission;
			uint16_t missionZone;
			uint16_t unknown3;
			uint16_t unknown4;
			uint16_t alienRace;
			uint16_t USOAttackTimer;
			uint16_t USOEscapeTimer;
			uint16_t status;
			uint32_t flags;
			uint16_t touchdownDepth;
			uint8_t unknown_ableToIntercept1;
			uint8_t unknown_ableToIntercept2;//Apparently, 'used to determine if XCraft are able to intercept'
			uint8_t unknown_ableToIntercept3;
			uint8_t unknown5;
		}__attribute__((packed))craft[50];
	};
	static_assert(sizeof(craft_dat) == 5500, "Expected craft_dat to be 5500 bytes");

	class diplom_dat
	{
	public:
		class 
		{
		public:
			uint16_t nameIdx;
			uint16_t satisfaction;
			int16_t funding[12];//Per month - jan to dec
			int16_t fundingChange;
			uint16_t willSignPact;
			uint16_t startingFunding;
			uint16_t fundingCap;
		}country[16];
	};
	static_assert(sizeof(diplom_dat) == 36*16, "Expected diplom_dat to be 576 bytes");

	class facil_dat
	{
	public:
		class
		{
		public:
			uint16_t stringOffset;
			int16_t price;
			uint8_t buildDays;
			uint8_t upkeep;
			uint8_t capacity;//??Has value in entries that can contain stuff (hangars, general stores, labs etc.)
			uint8_t unknown2;
			int16_t defenceStrength;
			uint8_t hitRatio;
			uint8_t unknown3;
			uint8_t buildFlag;
			uint8_t unknown4[3];
		}__attribute__((packed))facilities[17];

	};
	static_assert(sizeof(facil_dat) == 16*17, "Expected facl_dat to be 272 bytes");

	class iglob_dat
	{
	public:
		int32_t month;
		int32_t weekday;
		int32_t day;
		int32_t hour;
		int32_t minute;
		int32_t seconds;
		uint32_t unknown[9];
		uint32_t difficulty;
	};
	static_assert(sizeof(iglob_dat) == 64, "Expected iglob_dat to be 64 bytes");

	class inter_dat
	{
	public:
		class 
		{
		public:
			int16_t active;
			int16_t type;
			int16_t number;//??
			int16_t nextID;//??
			int16_t prevID;//??Possibly a linked-list for the window order??
			int16_t interStatus;
			int16_t ufoSize;
			int16_t unknown1[2];
			vec2_int16 windowPosition;
			int16_t statusMessageIdx;
			int16_t statusMessageCountdown;
			int16_t attackMode;
			int16_t distanceToTarget;
			int16_t requestedDistanceToTarget;
			int16_t speedDelta;
			int16_t speed;
			int16_t leftWeaponShotDistance[15];
			int16_t rightWeaponShotDistance[15];
			int16_t leftWeaponType;
			int16_t rightWeaponType;
			int16_t leftWeaponReloadTimer;
			int16_t rightWeaponReloadTimer;
			int16_t unknown2;
			int16_t UFOHitFlashTimer;
			int16_t UFOReturnFireIntenseLevel;
			int16_t hitCounter2;//??
			int16_t UFOViewOpen;
			int16_t XCOMCraftType;
			int16_t UFOCraftType;
			uint32_t attackingCraftIdx;
			uint32_t attackingCraftStatsPtr;
			uint32_t attackingCraftLeftWeaponStatsPtr;
			uint32_t attackingCraftRightWeaponStatsPtr;
			uint32_t UFOCraftIdx;
			uint32_t UFOCraftStatsPtr;
		}__attribute__((packed))interception[4];
	};
	static_assert(sizeof(inter_dat) == 568, "Expected inter_dat to be 568 bytes");

	class lease_dat
	{
	public:
		//Stores the geo-globe view info - angles are in 8ths of degrees (ie 13.3 fixed point)
		uint16_t YRotation;
		uint8_t unknown1[4];
		uint16_t XRotation;
		uint8_t unknown2[4];
		uint16_t zoom;//Seems to be the width of the globe sphere in pixels - zoom levels 90-120-180-360-450-720
		uint8_t unknown[166];
	};
	static_assert(sizeof(lease_dat) == 180, "Expected lease_dat to be 180 bytes");
	
	class liglob_dat
	{
	public:
		int32_t money;
		int32_t expenditureMonths[12];
		int32_t maintananceMonths[12];
		int32_t balanceMonths[12];
	};
	static_assert(sizeof(liglob_dat) == 148, "Expected liglob_dat to be 148 bytes");

	class loc_dat
	{
	public:
		class{
		public:
			uint8_t type;
			uint8_t ref;
			vec2_int16 position;
			int16_t siteCountdownHours;
			int16_t globeCoordinateFraction;//Stores the within-cell fraction until the next cell for moving objects
			int16_t count;//Suffix count (e.g. crash site-45 has 45 here)
			uint16_t unused1;
			uint8_t transferMode;//0=not transferred, 1=transferring, 2=selected to transfer
			uint8_t unused2;
			uint32_t flags;//bit 0=hidden, 1=moving, 2=targeted
		}locations[50];
	};
	static_assert(sizeof(loc_dat) == 1000, "Expected log_dat_entry to be 1000 bytes");

	class missions_dat
	{
	public:
		class
		{
		public:
			class{
			public:
				uint16_t UFOCount;//UFOs within the mission, ffff == not active
				uint16_t subUFOCount;//Only used for some missions?
				uint16_t timeCount;//30-mins to next ufo
				uint16_t race;//Alien race on mission
			}missions[7];
		}missionRegions[12];
	};
	static_assert(sizeof(missions_dat) == 672, "Expected missions_dat to be 672 bytes");

	class product_dat
	{
	public:
		class
		{
			int32_t cost;
			int16_t hours;
			int16_t workspace;
			uint8_t prereqs[3];
			uint8_t prereqCount[3];
			uint8_t typeIdx;
			uint8_t unused1;
			uint8_t canBuild;
			uint8_t unused2;
		}__attribute__((packed))products[MANUFACTURABLE_ITEM_COUNT];
	};
	static_assert(sizeof(product_dat) == 774, "Expected product_dat to be 774 bytes");

	static const int RESEARCH_ITEM_COUNT = 94;

	class project_dat
	{
	public:
		class
		{
		public:
			uint16_t daysRemaining[RESEARCH_ITEM_COUNT];//In scientist-days
			uint8_t  assignedScientists[RESEARCH_ITEM_COUNT];
			
		}base[8];
	};
	static_assert(sizeof(project_dat) == 2256, "Expected project_dat to be 2256 bytes");

	class purchase_dat
	{
	public:
		class
		{
		public:
			uint8_t category;
			uint8_t inventorySpaceIdx;//0 for personnel, craft = craft type
			uint32_t unknown1;//Set to 0xE8030000 for all but manufactured fraft
			int32_t buyPrice;
			int32_t sellPrice;
			uint8_t transferTimeHours;//Time needed for newly purchased items to arrive
			uint8_t unknown2;
			uint8_t previouslyPurchasedQuantity;//Not sure why this is stored/if it is used
			uint8_t unknown3;
			uint8_t purchasable;
			uint8_t unknown4;

		} __attribute__((packed))item[99];
	};
	static_assert(sizeof(purchase_dat) == 1980, "Expected purchase_dat to be 1980 bytes");

	class research_dat
	{
	public:
		class
		{
		public:
			int16_t researchTime;
			uint8_t topicName[3];//Byte0 is artifacts/autopsy/UFO components
			                     //B0=0 Byte 1 = live alien type, 2=rank
													 //b0=0, b1=0 b2 = xcom craft, heavy weapons, alien research
													 //b0=0, b1=0,b2=0 laser weapons
													 //if b0 or b1 is non-0, that named object is the pre-req to be in stores
			uint8_t unknown1;//non-0 for some live aliens?
			uint8_t UFOPaediaTopic;//0x0b is special for laser weapons, apparently
			uint8_t UFOPaediaEntry;
			uint8_t pointsAward;
			uint8_t unknown2;
			uint32_t completed;
			uint32_t requiredPrereq;
			uint32_t prereqsFulfilled;

		}__attribute__((packed))items[RESEARCH_ITEM_COUNT];
	};
	static_assert(sizeof(research_dat) == 2068, "Expected research_dat to be 2068 bytes");

	class saveinfo_dat
	{
	public:
		uint16_t startingBattle;//Only used for missdat save - 0 is beginning of new battlescape, if 1 check direct.dat
		uint8_t name[26];//Null-terminated name
		uint16_t year;
		uint16_t month;
		uint16_t day;
		uint16_t hour;
		uint16_t minute;
		uint16_t tacticalSave;//0=geoscape, 1=tactical
	};
	static_assert(sizeof(saveinfo_dat) == 40, "Expected saveinfo_dat to be 40 bytes");

	class site_dat
	{
	public:
		uint16_t lastShippingRoutes[5];
		uint16_t lastTerrorSites[12];
		uint16_t validLastShippingRoutes;
		uint16_t validLastTerrorSites;
		uint16_t currentAttackType;//'S' or 'T'
	};
	static_assert(sizeof(site_dat) == 40, "Expected site_dat to be 40 bytes");

	class soldier_dat
	{
	public:
		class
		{
		public:
			uint8_t MCSkillChange;
			uint8_t TUImprovement;
			uint8_t InMCImplantation;
			uint8_t Promotion;
			uint8_t HPImprovement;
			uint8_t StrengthImprovement;
			uint16_t baseRef;
			uint16_t craftRef;
			uint16_t rank;
			uint16_t preWoundCraftRef;//Seems unused?
			uint16_t kills;
			uint16_t wouldRecovery;
			uint16_t missions;
			uint16_t value;
			uint8_t transferBaseRef;
			uint8_t staminaImprovement;
			uint8_t reactionImprovement;
			uint8_t firingAccuracyImprovement;
			uint8_t initialTU;
			uint8_t initialHP;
			uint8_t initialStamina;
			uint8_t initialThrowingAccuracy;
			uint8_t armor;
			uint8_t gender;
			uint8_t race;
			uint8_t initialFiringAccuracy;
			uint8_t initialStrength;
			uint8_t name[27];
			uint8_t braveryImprovement;
			uint8_t throwingAccuracyImprovement;
			uint8_t initialBravery;
			uint8_t MCSkill;
			uint8_t meleeAccuracyImprovement;
			uint8_t MCStrength;
			uint8_t initialMeleeAccuracy;
			uint8_t initialReactions;
		}soldier[250];
	};
	static_assert(sizeof(soldier_dat) == 17500, "Expected soldier_dat to be 17500 bytes");

	class transfer_dat
	{
	public:
		class
		{
		public:
			uint8_t originBaseRef;//255==purchased so no base
			uint8_t destBaseRef;
			uint8_t transitHoursLeft;
			uint8_t type;//0=item,1=craft,2=soldier,3=scientists,4=engineers,5=alien
			uint16_t refNo;//Depends on type(above)
			               //0: item index (into base_dat)
										 //1: craft index (into geodata_dat)
										 //2: soldier index (into solder_dat)
										 //3&4: scientist&engineer - always 0
										 //5: alien index (into astore_dat)
			uint8_t qualtity;//0 here is an unused entry

		}transferItem[100];
	};
	static_assert(sizeof(transfer_dat) == 800, "Expected transfer_dat to be 800 bytes");

	class uiglob_dat
	{
	public:
		uint16_t nextUFONumber;
		uint16_t nextTerrorNumber;
		uint16_t nextAlienBaseNumber;
		uint16_t minimizedInterceptorWindows;//??
		uint16_t nextGroundAssaultNumber;
		uint16_t nextWaypointNumber;
		uint16_t nextTritonNumber;
		uint16_t nextHammerheadNumber;
		uint16_t nextLeviathonNumber;
		uint16_t nextBarracudaNumber;
		uint16_t nextMantaNumber;
		uint16_t currentGameYear;
		uint16_t monthlyScore[12];//Seems related to XCOM's research score (+ something else?)
	};
	static_assert(sizeof(uiglob_dat) == 48, "Expected uiglob_dat to be 48 bytes");

	static const int UFOPAEDIA_ENTRY_COUNT = 122;

	class up_dat
	{
	public:
		class
		{
		public:
			uint16_t categoryIdx;
			uint16_t entryIdx;
			uint8_t unknown1;
			uint8_t SPKIdx;
			uint16_t stringIdx;
			uint8_t researched;//0=not researched, 2=researched, 3=available from start
			uint8_t unknown2[3];
		}entry[UFOPAEDIA_ENTRY_COUNT];
	};
	static_assert(sizeof(up_dat) == 1464, "Expected up_dat to be 1464 bytes");

	class xbases_dat
	{
		public:
		class
		{
			uint16_t detected;
			uint16_t locIdx;
		}region[12];
	};
	static_assert(sizeof(xbases_dat) == 48, "Expected xbases_dat to be 48 bytes");

	class zonal_dat
	{
		uint8_t attackPriority[12];
	};
	static_assert(sizeof(zonal_dat) == 12, "Expected zonal_dat to be 12 bytes");

}//namespace datafiles
}//namespace ktftd

#endif //__DATAFILES_H
