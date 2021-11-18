#include <cstdint>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <functional>

enum DamageType {PhysicalDamage, FireDamage, ColdDamage, ShockDamage};
enum NPCStateTransitionType {EndConversationTransition, ContinueConversationTransition, ShopTransition};
enum ItemRarity {ResourceItem, NormalItem, RareItem, LegendaryItem, LeetItem};

class IPlayer;
class IActor;
class NPC;
class IItem;

struct NPCStateTransition {
    std::string text;
    NPCStateTransitionType type;
    std::string nextState;
};

struct NPCState {
    std::string text;
    std::vector<NPCStateTransition, std::allocator<NPCStateTransition> > transitions;
};

struct ItemAndCount {
    IItem *item;
    uint32_t count;
    uint32_t loadedAmmo;
};

struct ItemCountInfo {
    uint32_t count;
    uint32_t loadedAmmo;
};

struct Rotation {
    float pitch;
    float yaw;
    float roll;
  public:
    Rotation();
    Rotation(float, float, float);
};

struct QuestStateInfo {
    std::string state;
    uint32_t count;
};

struct Vector3 {
    float x;
    float y;
    float z;
  public:
    Vector3();
    Vector3(float, float, float);
    struct Vector3 operator*(float) const;
    struct Vector3 & operator*=(float);
    struct Vector3 operator+(const struct Vector3 &) const;
    struct Vector3 & operator+=(const struct Vector3 &);
    struct Vector3 operator-(const struct Vector3 &) const;
    struct Vector3 & operator-=(const struct Vector3 &);
    float MagnitudeSquared() const;
    float Magnitude() const;
    static float DistanceSquared(const struct Vector3 &, const struct Vector3 &);
    static float Distance(const struct Vector3 &, const struct Vector3 &);
    void Normalize();
    static struct Vector3 Normalize(const struct Vector3 &);
};

template<typename T>
class ActorRef {
  private:
    T *m_object;

  public:
    ActorRef();
    ActorRef(T *);
    ActorRef(const ActorRef<T> &);
    ActorRef<T> & operator=(T *);
    ActorRef<T> & operator=(const ActorRef<T> &);
    T * operator->() const;
    operator bool() const;
    operator NPC *() const;
    T * Get() const;
    bool operator<(const ActorRef<T> &) const;
};

class IItem {
  public:
    virtual ~IItem();
    virtual const char * GetName();
    virtual const char * GetDisplayName();
    virtual const char * GetItemTypeName();
    virtual const char * GetDescription();
    virtual const char * GetFlavorText();
    virtual bool CanEquip();
    virtual uint32_t GetMaximumCount();
    virtual bool CanActivate(IPlayer *);
    virtual bool CanActivateInInventory();
    virtual void Activate(IPlayer *);
    virtual bool ShowInInventory();
    virtual bool ShowEventOnPickup();
    virtual bool ShowEventOnDuplicatePickup();
    virtual bool ShowNotificationOnPickup();
    virtual float GetCooldownTime();
    virtual ItemRarity GetItemRarity();
    virtual IItem * GetAmmoType();
    virtual uint32_t GetClipSize();
    virtual int32_t GetDamage();
    virtual int32_t GetDamagePerSecond();
    virtual DamageType GetDamageType();
    virtual int32_t GetManaCost();
    virtual const char * GetCustomCostDescription();
    virtual bool IsAutoFire();
    virtual uint32_t GetNumberOfProjectiles();
    virtual float GetReloadTime(int32_t);
    virtual bool HasPartialReload();
    virtual float GetRange();
    virtual int32_t GetTradeValue();
    virtual bool IsDynamic();
    virtual bool IsUpdating();
};

class IQuestState {
  public:
    virtual const char * GetName();
    virtual const char * GetDescription();
    virtual void CheckForEarlyCompletion(IPlayer *);
    virtual void OnItemAcquired(IPlayer *, IItem *);
    virtual void OnItemPickupUsed(IPlayer *, const char *);
};

class IQuest {
  public:
    virtual const char * GetName();
    virtual const char * GetDescription();
    virtual IQuestState * GetStartingState();
    virtual IQuestState * GetStateByName(const char *);
};

class IUE4Actor {
  public:
    virtual void * GetUE4Actor();
    virtual void RemoveFromWorld();
    virtual struct Vector3 GetPosition();
    virtual struct Rotation GetRotation();
    virtual struct Vector3 GetProjectilePosition();
    virtual struct Vector3 GetCharacterVelocity();
    virtual void SetPosition(const struct Vector3 &);
    virtual void SetRotation(const struct Rotation &);
    virtual void SetCharacterVelocity(const struct Vector3 &);
    virtual void SetForwardAndStrafeMovement(float, float);
    virtual void InterpolatePositionAndRotation(const struct Vector3 &, const struct Rotation &, float, float);
    virtual bool MoveToLocation(const struct Vector3 &);
    virtual bool MoveToRandomLocationInRadius(float);
    virtual bool MoveToActor(IActor *);
    virtual void OnUpdateState(const char *, bool);
    virtual void OnTriggerEvent(const char *, IActor *);
    virtual void OnUpdatePvPEnabled(bool);
    virtual IActor * LineTraceTo(const struct Vector3 &);
    virtual void FireBullets(IItem *, int32_t, DamageType, const struct Vector3 &, uint32_t, float);
    virtual void LocalRespawn(const struct Vector3 &, const struct Rotation &);
    virtual bool IsOnGround();
    virtual void OnReload(uint32_t);
};

class IActor {
  public:
    virtual ~IActor();
    virtual void * GetUE4Actor();
    virtual bool IsNPC();
    virtual bool IsPlayer();
    virtual IPlayer * GetPlayerInterface();
    virtual void AddRef();
    virtual void Release();
    virtual void OnSpawnActor(IUE4Actor *);
    virtual void OnDestroyActor();
    virtual const char * GetBlueprintName();
    virtual bool IsCharacter();
    virtual bool CanBeDamaged(IActor *);
    virtual int32_t GetHealth();
    virtual int32_t GetMaxHealth();
    virtual void Damage(IActor *, IItem *, int32_t, DamageType);
    virtual void Tick(float);
    virtual bool CanUse(IPlayer *);
    virtual void OnUse(IPlayer *);
    virtual void OnHit(IActor *, const struct Vector3 &, const struct Vector3 &);
    virtual void OnAIMoveComplete();
    virtual const char * GetDisplayName();
    virtual bool IsElite();
    virtual bool IsPvPEnabled();
    virtual IItem ** GetShopItems(size_t &);
    virtual void FreeShopItems(IItem **);
    virtual int32_t GetBuyPriceForItem(IItem *);
    virtual int32_t GetSellPriceForItem(IItem *);
    virtual struct Vector3 GetLookPosition();
    virtual struct Rotation GetLookRotation();
    virtual IActor * GetOwner();
};

class ILocalPlayer : public IUE4Actor {
  public:
    virtual void SetPlayerInterface(IPlayer *);
    virtual struct Vector3 GetLookPosition();
    virtual struct Rotation GetLookRotation();
    virtual float GetForwardMovementFraction();
    virtual float GetStrafeMovementFraction();
    virtual void SetCurrentQuest(IQuest *, IQuestState *, uint32_t);
    virtual void DisplayMessage(const char *, const char *);
    virtual void DisplayEvent(const char *, const char *);
    virtual void OnEquip(size_t, IItem *);
    virtual void OnChangeSlot(size_t);
    virtual void OnUpdateCountdown(int32_t);
    virtual void OnUpdatePvPCountdown(bool, int32_t);
    virtual void OnNewItem(const char *, uint32_t);
    virtual void OnNPCConversationState(IActor *, const char *, const char **, const char **, size_t);
    virtual void OnNPCConversationEnd();
    virtual void OnNPCShop(IActor *);
    virtual void OnChatMessage(const char *, bool, const char *);
    virtual void OnPlayerKillMessage(const char *, bool, const char *, bool, IItem *);
    virtual void OnPlayerSuicideMessage(const char *, bool, IItem *);
    virtual void OnPlayerDeadMessage(const char *, bool, const char *);
    virtual void OnAchievement(const char *);
    virtual void OnLocalDeath(IActor *, IItem *);
};

class Spawner;
class TimerSet;


class IAchievement {
  public:
    virtual const char * GetName();
    virtual const char * GetDisplayName();
    virtual const char * GetDescription();
};

class IFastTravel {
  public:
    virtual ~IFastTravel();
    virtual size_t GetCount();
    virtual const char * GetRegionName(size_t);
    virtual const char * GetDisplayName(size_t);
    virtual void Destroy();
};

class IInventory {
  public:
    virtual ~IInventory();
    virtual size_t GetCount();
    virtual IItem * GetItem(size_t);
    virtual uint32_t GetItemCount(size_t);
    virtual uint32_t GetItemLoadedAmmo(size_t);
    virtual void Destroy();
};

class IPlayer {
  public:
    virtual  IActor * GetActorInterface();
    void AddRef();
    void Release();
    virtual bool IsLocalPlayer() const;
    virtual  ILocalPlayer * GetLocalPlayer() const;
    virtual const char * GetPlayerName();
    virtual const char * GetTeamName();
    virtual uint8_t GetAvatarIndex();
    virtual const uint32_t * GetColors();
    virtual bool IsPvPDesired();
    virtual void SetPvPDesired(bool);
    virtual  IInventory * GetInventory();
    virtual uint32_t GetItemCount( IItem *);
    virtual uint32_t GetLoadedAmmo( IItem *);
    virtual bool AddItem( IItem *, uint32_t, bool);
    virtual bool RemoveItem( IItem *, uint32_t);
    virtual bool AddLoadedAmmo( IItem *,  IItem *, uint32_t);
    virtual bool RemoveLoadedAmmo(IItem *, uint32_t);
    virtual IItem * GetItemForSlot(size_t);
    virtual void EquipItem(size_t, IItem *);
    virtual size_t GetCurrentSlot();
    virtual void SetCurrentSlot(size_t);
    virtual IItem * GetCurrentItem();
    virtual int32_t GetMana();
    virtual bool UseMana(int32_t);
    virtual void SetItemCooldown(IItem *, float, bool);
    virtual bool IsItemOnCooldown(IItem *);
    virtual float GetItemCooldown(IItem *);
    virtual bool HasPickedUp(const char *);
    virtual void MarkAsPickedUp(const char *);
    virtual IQuest ** GetQuestList(size_t *);
    virtual void FreeQuestList(IQuest **);
    virtual IQuest * GetCurrentQuest();
    virtual void SetCurrentQuest(IQuest *);
    virtual struct PlayerQuestState GetStateForQuest(IQuest *);
    virtual void StartQuest(IQuest *);
    virtual void AdvanceQuestToState(IQuest *, IQuestState *);
    virtual void CompleteQuest(IQuest *);
    virtual bool IsQuestStarted(IQuest *);
    virtual bool IsQuestCompleted(IQuest *);
    virtual void EnterAIZone(const char *);
    virtual void ExitAIZone(const char *);
    virtual void UpdateCountdown(int32_t);
    void HideCountdown();
    virtual bool CanReload();
    virtual void RequestReload();
    virtual float GetWalkingSpeed();
    virtual float GetSprintMultiplier();
    virtual float GetJumpSpeed();
    virtual float GetJumpHoldTime();
    virtual bool CanJump();
    virtual void SetJumpState(bool);
    virtual void SetSprintState(bool);
    virtual void SetFireRequestState(bool);
    virtual void TransitionToNPCState(const char *);
    virtual void BuyItem(IActor *, IItem *, uint32_t);
    virtual void SellItem(IActor *, IItem *, uint32_t);
    virtual void EnterRegion(const char *);
    virtual void Respawn();
    virtual void Teleport(const char *);
    virtual void Chat(const char *);
    virtual IFastTravel * GetFastTravelDestinations(const char *);
    virtual void FastTravel(const char *, const char *);
    virtual void MarkAsAchieved(IAchievement *);
    virtual bool HasAchieved(IAchievement *);
    virtual void SubmitDLCKey(const char *);
    virtual uint32_t GetCircuitInputs(const char *);
    virtual void SetCircuitInputs(const char *, uint32_t);
    virtual void GetCircuitOutputs(const char *, bool *, size_t);
};


class Actor : public IActor {
  protected:
    size_t m_refs;
    uint32_t m_id;
    IUE4Actor *m_target;
    TimerSet *m_timers;
    std::string m_blueprintName;
    ActorRef<IActor> m_owner;
    int32_t m_health;
    std::map<std::basic_string<char>, bool, std::less<std::basic_string<char> >, std::allocator<std::pair<std::basic_string<char> const, bool> > > m_states;
    float m_forwardMovementFraction;
    float m_strafeMovementFraction;
    struct Vector3 m_remotePosition;
    struct Vector3 m_remoteVelocity;
    struct Rotation m_remoteRotation;
    float m_remoteLocationBlendFactor;
    Spawner *m_spawner;

    virtual void OnKilled(IActor *, IItem *);
    virtual void OnTargetKilled(IActor *, IItem *);
  public:
    Actor(const std::string &);
    virtual ~Actor();
    virtual bool IsValid() const;
    virtual void * GetUE4Actor();
    virtual void AddRef();
    virtual void Release();
    void RemoveFromWorld();
    virtual void OnSpawnActor(IUE4Actor *);
    virtual void OnDestroyActor();
    virtual std::string GetDeathMessage();
    virtual const char * GetBlueprintName();
    virtual bool IsCharacter();
    virtual bool IsNPC();
    virtual bool IsProjectile();
    virtual bool IsPlayer();
    virtual IPlayer * GetPlayerInterface();
    virtual bool ShouldSendPositionUpdates();
    virtual bool ShouldReceivePositionUpdates();
    uint32_t GetId() const;
    void SetId(uint32_t);
    struct Vector3 GetPosition();
    struct Vector3 GetProjectilePosition();
    virtual struct Vector3 GetLookPosition();
    struct Rotation GetRotation();
    virtual struct Rotation GetLookRotation();
    struct Vector3 GetVelocity();
    float GetForwardMovementFraction() const;
    float GetStrafeMovementFraction() const;
    bool IsOnGround();
    void SetPosition(const struct Vector3 &);
    void SetRotation(const struct Rotation &);
    void SetVelocity(const struct Vector3 &);
    void SetForwardAndStrafeMovement(float, float);
    void SetRemotePositionAndRotation(const struct Vector3 &, const struct Rotation &);
    void InterpolateRemotePosition(float);
    virtual IActor * GetOwner();
    void LocalRespawn(const struct Vector3 &, const struct Rotation &);
    bool MoveToLocation(const struct Vector3 &);
    bool MoveToRandomLocationInRadius(float);
    bool MoveToActor(IActor *);
    bool GetState(const std::string &);
    virtual void UpdateState(const std::string &, bool);
    virtual void TriggerEvent(const std::string &, IActor *, bool);
    const std::map<std::basic_string<char>, bool, std::less<std::basic_string<char> >, std::allocator<std::pair<std::basic_string<char> const, bool> > > & GetStates();
    IActor * LineTraceTo(const struct Vector3 &);
    void FireBullets(IItem *, int32_t, DamageType, float, uint32_t, float);
    void FireBullets(IItem *, int32_t, DamageType, const struct Vector3 &, float, uint32_t, float);
    virtual bool CanBeDamaged(IActor *);
    virtual float GetMaximumDamageDistance();
    virtual int32_t GetHealth();
    virtual int32_t GetMaxHealth();
    virtual void Damage(IActor *, IItem *, int32_t, DamageType);
    void PerformSetHealth(int32_t);
    virtual void Tick(float);
    virtual bool CanUse(IPlayer *);
    virtual void OnUse(IPlayer *);
    virtual void PerformUse(IPlayer *);
    virtual void OnHit(IActor *, const struct Vector3 &, const struct Vector3 &);
    virtual void OnAIMoveComplete();
    virtual const char * GetDisplayName();
    virtual bool IsElite();
    virtual bool IsPvPEnabled();
    virtual IItem ** GetShopItems(size_t &);
    virtual std::vector<IItem*, std::allocator<IItem*> > GetShopItems();
    virtual void FreeShopItems(IItem **);
    virtual std::vector<IItem*, std::allocator<IItem*> > GetValidBuyItems();
    virtual float GetShopBuyPriceMultiplier();
    virtual float GetShopSellPriceMultiplier();
    virtual int32_t GetBuyPriceForItem(IItem *);
    virtual int32_t GetSellPriceForItem(IItem *);
    void SetSpawner(Spawner *);
    void AddTimer(const std::string &, float, const std::function<void ()> &);
    void AddTimerWithContext(const std::string &, float, const std::function<void (Actor *)> &);
    void AddRecurringTimer(const std::string &, float, const std::function<void ()> &);
    void AddRecurringTimerWithContext(const std::string &, float, const std::function<void (Actor *)> &);
    void CancelTimer(const std::string &);
    void PerformReloadNotification(uint32_t);
};

class WriteStream {
  private:
    class Socket *m_sock;
    std::vector<unsigned char> m_buffer;

  public:
    WriteStream(class Socket *);
    void SetSocket(class Socket *);
    void Write8(uint8_t);
    void Write16(uint16_t);
    void Write32(uint32_t);
    void Write64(uint64_t);
    void WriteSaturated16(float);
    void WriteString(const std::string &);
    void WriteFloat(float);
    void WriteVector(const struct Vector3 &);
    void WriteVector16(const struct Vector3 &);
    void WriteRotation(const struct Rotation &);
    void WritePrecisionRotation(const struct Rotation &);
    void WriteSignedFraction(float);
    void Write(const class WriteStream &);
    void Write(const void *, size_t);
    void Flush();
    void Clear();
};

class Player : public Actor, public IPlayer {
  private:
    uint32_t m_characterId;
    std::string m_playerName;
    std::string m_teamName;
    uint8_t m_avatarIndex;
    uint32_t m_colors[4];
    std::map<IItem*, ItemAndCount, std::less<IItem*>, std::allocator<std::pair<IItem* const, ItemAndCount> > > m_inventory;
    std::set<std::basic_string<char>, std::less<std::basic_string<char> >, std::allocator<std::basic_string<char> > > m_pickups;
    std::map<IItem*, float, std::less<IItem*>, std::allocator<std::pair<IItem* const, float> > > m_cooldowns;
    std::map<std::basic_string<char>, unsigned int, std::less<std::basic_string<char> >, std::allocator<std::pair<std::basic_string<char> const, unsigned int> > > m_circuitInputs;
    std::map<std::basic_string<char>, std::vector<std::allocator<bool>>, std::less<std::basic_string<char> >, std::allocator<std::pair<std::basic_string<char> const, std::vector<std::allocator<bool>> > > > m_circuitOutputs;
    bool m_admin;
    bool m_pvpEnabled;
    bool m_pvpDesired;
    float m_pvpChangeTimer;
    int32_t m_pvpChangeReportedTimer;
    bool m_changingServerRegion;
    std::string m_currentRegion;
    std::string m_changeRegionDestination;
    std::set<std::basic_string<char>, std::less<std::basic_string<char> >, std::allocator<std::basic_string<char> > > m_aiZones;
    int32_t m_mana;
    float m_manaRegenTimer;
    float m_healthRegenCooldown;
    float m_healthRegenTimer;
    int32_t m_countdown;
    struct Vector3 m_remoteLookPosition;
    struct Rotation m_remoteLookRotation;
    IItem *m_equipped[10];
    size_t m_currentSlot;
    std::map<IQuest*, PlayerQuestState, std::less<IQuest*>, std::allocator<std::pair<IQuest* const, PlayerQuestState> > > m_questStates;
    IQuest *m_currentQuest;
    float m_walkingSpeed;
    float m_jumpSpeed;
    float m_jumpHoldTime;
    ActorRef<NPC> m_currentNPC;
    std::string m_currentNPCState;
    ILocalPlayer *m_localPlayer;
    WriteStream *m_eventsToSend;
    bool m_itemsUpdated;
    float m_itemSyncTimer;
    uint32_t m_chatMessageCounter;
    float m_chatFloodDecayTimer;
    IItem *m_lastHitByItem;
    float m_lastHitItemTimeLeft;
    float m_circuitStateCooldownTimer;

  protected:
    virtual void OnKilled(IActor *, IItem *);
  public:
    Player(bool);
    virtual ~Player();
    virtual bool IsPlayer();
    virtual IPlayer * GetPlayerInterface();
    virtual IActor * GetActorInterface();
    virtual bool CanBeDamaged(IActor *);
    virtual bool IsCharacter();
    virtual bool ShouldSendPositionUpdates();
    virtual bool ShouldReceivePositionUpdates();
    virtual void Tick(float);
    virtual void Damage(IActor *, IItem *, int32_t, DamageType);
    virtual void OnDestroyActor();
    void OnKillEvent(IPlayer *, IActor *, IItem *);
    virtual struct Vector3 GetLookPosition();
    virtual struct Rotation GetLookRotation();
    void SetRemoteLookPosition(const struct Vector3 &);
    void SetRemoteLookRotation(const struct Rotation &);
    virtual bool CanJump();
    virtual bool IsLocalPlayer() const;
    virtual ILocalPlayer * GetLocalPlayer() const;
    void InitLocalPlayer(ILocalPlayer *);
    bool IsAdmin() const;
    void SetPlayerName(const std::string &);
    void SetTeamName(const std::string &);
    void SetAvatarIndex(uint8_t);
    void SetColors(const uint32_t *);
    void SetCharacterId(uint32_t);
    virtual bool IsPvPEnabled();
    virtual bool IsPvPDesired();
    virtual void SetPvPDesired(bool);
    void PerformSetPvPEnabled(bool);
    void PerformSetPvPDesired(bool);
    void PerformUpdatePvPCountdown(bool, int32_t);
    virtual void UpdateState(const std::string &, bool);
    virtual const char * GetPlayerName();
    virtual const char * GetTeamName();
    virtual uint8_t GetAvatarIndex();
    virtual const uint32_t * GetColors();
    uint32_t GetCharacterId() const;
    const std::map<IItem*, ItemAndCount, std::less<IItem*>, std::allocator<std::pair<IItem* const, ItemAndCount> > > & GetItemList() const;
    virtual IInventory * GetInventory();
    virtual uint32_t GetItemCount(IItem *);
    virtual uint32_t GetLoadedAmmo(IItem *);
    virtual bool AddItem(IItem *, uint32_t, bool);
    virtual bool RemoveItem(IItem *, uint32_t);
    bool PerformAddItem(IItem *, uint32_t, bool);
    bool PerformRemoveItem(IItem *, uint32_t);
    virtual bool AddLoadedAmmo(IItem *, IItem *, uint32_t);
    virtual bool RemoveLoadedAmmo(IItem *, uint32_t);
    void PerformSetLoadedAmmo(IItem *, uint32_t);
    virtual IItem * GetItemForSlot(size_t);
    virtual void EquipItem(size_t, IItem *);
    void PerformEquipItem(size_t, IItem *);
    virtual size_t GetCurrentSlot();
    virtual IItem * GetCurrentItem();
    virtual void SetCurrentSlot(size_t);
    void PerformSetCurrentSlot(size_t);
    void SetRemoteItem(IItem *);
    virtual int32_t GetMana();
    virtual bool UseMana(int32_t);
    void PerformSetMana(int32_t);
    virtual void SetItemCooldown(IItem *, float, bool);
    virtual bool IsItemOnCooldown(IItem *);
    virtual float GetItemCooldown(IItem *);
    virtual bool HasPickedUp(const char *);
    virtual void MarkAsPickedUp(const char *);
    void PerformMarkAsPickedUp(const std::string &);
    virtual IQuest ** GetQuestList(size_t *);
    virtual void FreeQuestList(IQuest **);
    virtual IQuest * GetCurrentQuest();
    virtual struct PlayerQuestState GetStateForQuest(IQuest *);
    virtual bool IsQuestStarted(IQuest *);
    virtual bool IsQuestCompleted(IQuest *);
    virtual void SetCurrentQuest(IQuest *);
    virtual void StartQuest(IQuest *);
    virtual void AdvanceQuestToState(IQuest *, IQuestState *);
    virtual void CompleteQuest(IQuest *);
    void PerformSetCurrentQuest(IQuest *);
    void PerformStartQuest(IQuest *);
    void PerformAdvanceQuestToState(IQuest *, IQuestState *);
    void PerformCompleteQuest(IQuest *);
    void SetInitialQuestStates(const std::map<std::basic_string<char>, QuestStateInfo, std::less<std::basic_string<char> >, std::allocator<std::pair<std::basic_string<char> const, QuestStateInfo> > > &, const std::string &);
    void SetInitialItemState(const std::map<std::basic_string<char>, ItemCountInfo, std::less<std::basic_string<char> >, std::allocator<std::pair<std::basic_string<char> const, ItemCountInfo> > > &, 
        const std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > > &, uint8_t);
    void SetInitialPickupState(const std::set<std::basic_string<char>, std::less<std::basic_string<char> >, std::allocator<std::basic_string<char> > > &);
    virtual void EnterAIZone(const char *);
    virtual void ExitAIZone(const char *);
    virtual void UpdateCountdown(int32_t);
    void PerformUpdateCountdown(int32_t);
    virtual void TriggerEvent(const std::string &, IActor *, bool);
    virtual bool CanReload();
    virtual void RequestReload();
    void PerformRequestReload();
    virtual float GetWalkingSpeed();
    virtual float GetSprintMultiplier();
    virtual float GetJumpSpeed();
    virtual float GetJumpHoldTime();
    virtual void SetJumpState(bool);
    virtual void SetSprintState(bool);
    virtual void SetFireRequestState(bool);
    void SetCurrentNPCState(NPC *, const std::string &);
    void EndNPCConversation();
    void EnterNPCShop(NPC *);
    NPC * GetCurrentNPC() const;
    const std::string & GetCurrentNPCState() const;
    virtual void TransitionToNPCState(const char *);
    void PerformTransitionToNPCState(const std::string &);
    virtual void BuyItem(IActor *, IItem *, uint32_t);
    void PerformBuyItem(IActor *, IItem *, uint32_t);
    virtual void SellItem(IActor *, IItem *, uint32_t);
    void PerformSellItem(IActor *, IItem *, uint32_t);
    virtual void EnterRegion(const char *);
    bool IsChangingRegion() const;
    const std::string & GetChangeRegionDestination() const;
    void PerformEnterRegion(const std::string &);
    struct LocationAndRotation GetSpawnLocation();
    virtual void Respawn();
    void PerformRespawn();
    void PerformRespawnAtLocation(const struct Vector3 &, const struct Rotation &);
    virtual void Teleport(const char *);
    void PerformTeleport(const std::string &);
    virtual void SendEvent(const WriteStream &);
    virtual void WriteAllEvents(WriteStream &);
    void SyncItems();
    virtual void Chat(const char *);
    void PerformChat(const std::string &);
    void ReceiveChat(Player *, const std::string &);
    virtual IFastTravel * GetFastTravelDestinations(const char *);
    virtual void FastTravel(const char *, const char *);
    void PerformFastTravel(const std::string &, const std::string &);
    void OnTravelComplete(const std::string &);
    IItem * GetLastHitByItem() const;
    void PerformSetLastHitByItem(IItem *);
    virtual void MarkAsAchieved(IAchievement *);
    virtual bool HasAchieved(IAchievement *);
    virtual void SubmitDLCKey(const char *);
    void PerformSubmitDLCKey(const std::string &);
    virtual uint32_t GetCircuitInputs(const char *);
    virtual void SetCircuitInputs(const char *, uint32_t);
    void PerformSetCircuitInputs(const std::string &, uint32_t);
    virtual void GetCircuitOutputs(const char *, bool *, size_t);
    void PerformSetCircuitOutputs(const std::string &, std::vector<std::allocator<bool>>);
    void InitCircuitStates();
};

class NPC : public Actor {
  private:
    std::map<std::string, NPCState, std::less<std::string>, std::allocator<std::pair<std::string const, NPCState> > > m_states;

  public:
    NPC(const std::string &);
    virtual bool IsNPC();
    void AddState(const std::string &, const std::string &);
    void AddStateTransition(const std::string &, const std::string &, const std::string &);
    void AddStateTransitionToEnd(const std::string &, const std::string &);
    void AddStateTransitionToShop(const std::string &, const std::string &);
    std::string GetTextForState(const std::string &);
    std::vector<NPCStateTransition, std::allocator<NPCStateTransition> > GetTransitionsForState(const std::string &);
    virtual std::string GetInitialState(IPlayer *);
    virtual void OnTransitionTaken(IPlayer *, const std::string &, const std::string &);
    virtual bool CanUse(IPlayer *);
    virtual void PerformUse(IPlayer *);
    virtual int32_t GetBuyPriceForItem(IItem *);
    virtual int32_t GetSellPriceForItem(IItem *);
};

class AIZone;

class AIZoneListener {
  private:
    AIZone *m_zone;

  public:
    AIZoneListener();
    virtual ~AIZoneListener();
    void SetAIZone(const std::string &);
    virtual void OnAIZoneActivated();
    virtual void OnAIZoneDeactivated();
    bool IsAIZoneActive();
};

class AIZone {
  private:
    std::string m_name;
    size_t m_playerCount;
    std::set<AIZoneListener*, std::less<AIZoneListener*>, std::allocator<AIZoneListener*> > m_listeners;

  public:
    AIZone(const std::string &);
    const std::string & GetName() const;
    bool IsActive() const;
    void OnPlayerEntered();
    void OnPlayerLeft();
    void AddListener(AIZoneListener *);
    void RemoveListener(AIZoneListener *);
};

class Spawner : public AIZoneListener {
  protected:
    std::vector<ActorRef<Actor>, std::allocator<ActorRef<Actor> > > m_actors;
    struct Vector3 m_position;
    struct Rotation m_rotation;
    size_t m_maxActors;
    float m_maxSpawnTimer;
    float m_currentSpawnTimer;

  public:
    Spawner(const std::string &, const struct Vector3 &, const struct Rotation &, size_t, float);
    virtual void OnAIZoneActivated();
    virtual void OnAIZoneDeactivated();
    virtual void Tick(float);
    virtual Actor * Spawn();
    void RemoveActor(Actor *);
    virtual size_t GetMaxActors();
    virtual float GetSpawnTimer();
};

class TimerSet {
  struct TimerEvent;
  private:
    std::map<std::string, TimerSet::TimerEvent, std::less<std::string>, std::allocator<std::pair<std::string const, TimerSet::TimerEvent> > > m_timers;

  public:
    void Add(const std::string &, float, const std::function<void ()> &);
    void AddWithContext(const std::string &, float, const std::function<void (Actor *)> &);
    void AddRecurring(const std::string &, float, const std::function<void ()> &);
    void AddRecurringWithContext(const std::string &, float, const std::function<void (Actor *)> &);
    void Cancel(const std::string &);
    void Clear();
    void Tick(Actor *, float);
};