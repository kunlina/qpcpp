//****************************************************************************
// Model: game.qm
// File:  ./mine2.cpp
//
// This code has been generated by QM tool (see state-machine.com/qm).
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details.
//****************************************************************************
//${.::mine2.cpp} ............................................................
#include "qp_port.h"
#include "bsp.h"
#include "game.h"

Q_DEFINE_THIS_FILE

// encapsulated delcaration of the Mine1 HSM ---------------------------------
namespace GAME {

//${AOs::Mine2} ..............................................................
class Mine2 : public QP::QMsm {
private:
    uint8_t m_x;
    uint8_t m_y;
    uint8_t m_exp_ctr;

public:
    Mine2()
      : QMsm(Q_STATE_CAST(&initial))
    {}

protected:
    static QP::QState initial(Mine2 * const me, QP::QEvt const * const e);
    static QP::QState unused  (Mine2 * const me, QP::QEvt const * const e);
    static QP::QMState const unused_s;
    static QP::QState used  (Mine2 * const me, QP::QEvt const * const e);
    static QP::QState used_x(Mine2 * const me);
    static QP::QMState const used_s;
    static QP::QState exploding  (Mine2 * const me, QP::QEvt const * const e);
    static QP::QState exploding_e(Mine2 * const me);
    static QP::QMState const exploding_s;
    static QP::QState planted  (Mine2 * const me, QP::QEvt const * const e);
    static QP::QMState const planted_s;
};

} // namespace GAME

namespace GAME {

// local objects -------------------------------------------------------------
static Mine2 l_mine2[GAME_MINES_MAX]; // a pool of type-2 mines

//............................................................................
QP::QMsm *Mine2_getInst(uint8_t id) {
    Q_REQUIRE(id < GAME_MINES_MAX);
    return &l_mine2[id];
}

// helper function to provide the ID of this mine ............................
static inline uint8_t MINE_ID(Mine2 const * const me) {
    return static_cast<uint8_t>(me - l_mine2);
}

} // namespace GAME

// Mine1 class definition ----------------------------------------------------
namespace GAME {

//${AOs::Mine2} ..............................................................

//${AOs::Mine2::SM} ..........................................................
QP::QState Mine2::initial(Mine2 * const me, QP::QEvt const * const e) {
    static QP::QMTranActTable const tatbl_ = { // transition-action table
        &unused_s,
        {
            Q_ACTION_CAST(0)  // zero terminator
        }
    };
    // ${AOs::Mine2::SM::initial}
    static bool dict_sent = false;
    if (!dict_sent) {
        dict_sent = true;
        QS_OBJ_DICTIONARY(&l_mine2[0]); // obj. dictionaries for Mine2 pool
        QS_OBJ_DICTIONARY(&l_mine2[1]);
        QS_OBJ_DICTIONARY(&l_mine2[2]);
        QS_OBJ_DICTIONARY(&l_mine2[3]);
        QS_OBJ_DICTIONARY(&l_mine2[4]);

        QS_FUN_DICTIONARY(&Mine2::initial);// fun. dictionaries for Mine2 HSM
        QS_FUN_DICTIONARY(&Mine2::unused);
        QS_FUN_DICTIONARY(&Mine2::used);
        QS_FUN_DICTIONARY(&Mine2::planted);
        QS_FUN_DICTIONARY(&Mine2::exploding);
    }
    // local signals
    QS_SIG_DICTIONARY(MINE_PLANT_SIG,    me);
    QS_SIG_DICTIONARY(MINE_DISABLED_SIG, me);
    QS_SIG_DICTIONARY(MINE_RECYCLE_SIG,  me);
    QS_SIG_DICTIONARY(SHIP_IMG_SIG,      me);
    QS_SIG_DICTIONARY(MISSILE_IMG_SIG,   me);
    return QM_TRAN_INIT(&tatbl_);
}
//${AOs::Mine2::SM::unused} ..................................................
QP::QMState const Mine2::unused_s = {
    static_cast<QP::QMState const *>(0), // superstate (top)
    Q_STATE_CAST(&unused),
    Q_ACTION_CAST(0), // no entry action
    Q_ACTION_CAST(0), // no exit action
    Q_ACTION_CAST(0)  // no intitial tran.
};
// ${AOs::Mine2::SM::unused}
QP::QState Mine2::unused(Mine2 * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // ${AOs::Mine2::SM::unused::MINE_PLANT}
        case MINE_PLANT_SIG: {
            static QP::QMTranActTable const tatbl_ = { // transition-action table
                &planted_s,
                {
                    Q_ACTION_CAST(0)  // zero terminator
                }
            };
            me->m_x = Q_EVT_CAST(ObjectPosEvt)->x;
            me->m_y = Q_EVT_CAST(ObjectPosEvt)->y;
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
//${AOs::Mine2::SM::used} ....................................................
QP::QMState const Mine2::used_s = {
    static_cast<QP::QMState const *>(0), // superstate (top)
    Q_STATE_CAST(&used),
    Q_ACTION_CAST(0), // no entry action
    Q_ACTION_CAST(&used_x),
    Q_ACTION_CAST(0)  // no intitial tran.
};
// ${AOs::Mine2::SM::used}
QP::QState Mine2::used_x(Mine2 * const me) {
    // tell the Tunnel that this mine is becoming disabled
    MineEvt *mev = Q_NEW(MineEvt, MINE_DISABLED_SIG);
    mev->id = MINE_ID(me);
    AO_Tunnel->POST(mev, me);
    return QM_EXIT(&used_s);
}
// ${AOs::Mine2::SM::used}
QP::QState Mine2::used(Mine2 * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // ${AOs::Mine2::SM::used::MINE_RECYCLE}
        case MINE_RECYCLE_SIG: {
            static struct {
                QP::QMState const *target;
                QP::QActionHandler act[2];
            } const tatbl_ = { // transition-action table
                &unused_s,
                {
                    Q_ACTION_CAST(&used_x), // exit
                    Q_ACTION_CAST(0)  // zero terminator
                }
            };
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
//${AOs::Mine2::SM::used::exploding} .........................................
QP::QMState const Mine2::exploding_s = {
    &Mine2::used_s, // superstate
    Q_STATE_CAST(&exploding),
    Q_ACTION_CAST(&exploding_e),
    Q_ACTION_CAST(0), // no exit action
    Q_ACTION_CAST(0)  // no intitial tran.
};
// ${AOs::Mine2::SM::used::exploding}
QP::QState Mine2::exploding_e(Mine2 * const me) {
    me->m_exp_ctr = 0U;
    return QM_ENTRY(&exploding_s);
}
// ${AOs::Mine2::SM::used::exploding}
QP::QState Mine2::exploding(Mine2 * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // ${AOs::Mine2::SM::used::exploding::TIME_TICK}
        case TIME_TICK_SIG: {
            // ${AOs::Mine2::SM::used::exploding::TIME_TICK::[stillonscreen?]}
            if ((me->m_x >= GAME_SPEED_X) && (me->m_exp_ctr < 15)) {
                ++me->m_exp_ctr;  // advance the explosion counter
                me->m_x -= GAME_SPEED_X; // move explosion by 1 step

                // tell the Game to render the current stage of Explosion
                ObjectImageEvt *oie = Q_NEW(ObjectImageEvt, EXPLOSION_SIG);
                oie->x   = me->m_x + 1U;  // x of explosion
                oie->y   = (int8_t)((int)me->m_y - 4 + 2); // y of explosion
                oie->bmp = EXPLOSION0_BMP + (me->m_exp_ctr >> 2);
                AO_Tunnel->POST(oie, me);
                status_ = QM_HANDLED();
            }
            // ${AOs::Mine2::SM::used::exploding::TIME_TICK::[else]}
            else {
                static struct {
                    QP::QMState const *target;
                    QP::QActionHandler act[2];
                } const tatbl_ = { // transition-action table
                    &unused_s,
                    {
                        Q_ACTION_CAST(&used_x), // exit
                        Q_ACTION_CAST(0)  // zero terminator
                    }
                };
                status_ = QM_TRAN(&tatbl_);
            }
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
//${AOs::Mine2::SM::used::planted} ...........................................
QP::QMState const Mine2::planted_s = {
    &Mine2::used_s, // superstate
    Q_STATE_CAST(&planted),
    Q_ACTION_CAST(0), // no entry action
    Q_ACTION_CAST(0), // no exit action
    Q_ACTION_CAST(0)  // no intitial tran.
};
// ${AOs::Mine2::SM::used::planted}
QP::QState Mine2::planted(Mine2 * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // ${AOs::Mine2::SM::used::planted::TIME_TICK}
        case TIME_TICK_SIG: {
            // ${AOs::Mine2::SM::used::planted::TIME_TICK::[me->m_x>=GAME~]}
            if (me->m_x >= GAME_SPEED_X) {
                me->m_x -= GAME_SPEED_X; // move the mine 1 step
                // tell the Tunnel to draw the Mine
                ObjectImageEvt *oie = Q_NEW(ObjectImageEvt, MINE_IMG_SIG);
                oie->x   = me->m_x;
                oie->y   = me->m_y;
                oie->bmp = MINE2_BMP;
                AO_Tunnel->POST(oie, me);
                status_ = QM_HANDLED();
            }
            // ${AOs::Mine2::SM::used::planted::TIME_TICK::[else]}
            else {
                static struct {
                    QP::QMState const *target;
                    QP::QActionHandler act[2];
                } const tatbl_ = { // transition-action table
                    &unused_s,
                    {
                        Q_ACTION_CAST(&used_x), // exit
                        Q_ACTION_CAST(0)  // zero terminator
                    }
                };
                status_ = QM_TRAN(&tatbl_);
            }
            break;
        }
        // ${AOs::Mine2::SM::used::planted::SHIP_IMG}
        case SHIP_IMG_SIG: {
            uint8_t x   = Q_EVT_CAST(ObjectImageEvt)->x;
            uint8_t y   = Q_EVT_CAST(ObjectImageEvt)->y;
            uint8_t bmp = Q_EVT_CAST(ObjectImageEvt)->bmp;
            // ${AOs::Mine2::SM::used::planted::SHIP_IMG::[collisionwith~]}
            if (do_bitmaps_overlap(MINE2_BMP, me->m_x, me->m_y, bmp, x, y)) {
                static struct {
                    QP::QMState const *target;
                    QP::QActionHandler act[2];
                } const tatbl_ = { // transition-action table
                    &unused_s,
                    {
                        Q_ACTION_CAST(&used_x), // exit
                        Q_ACTION_CAST(0)  // zero terminator
                    }
                };
                static MineEvt const mine2_hit(HIT_MINE_SIG, 2U);
                AO_Ship->POST(&mine2_hit, me);
                // go straight to 'disabled' and let the Ship do
                // the exploding
                status_ = QM_TRAN(&tatbl_);
            }
            else {
                status_ = QM_UNHANDLED();
            }
            break;
        }
        // ${AOs::Mine2::SM::used::planted::MISSILE_IMG}
        case MISSILE_IMG_SIG: {
            uint8_t x   = Q_EVT_CAST(ObjectImageEvt)->x;
            uint8_t y   = Q_EVT_CAST(ObjectImageEvt)->y;
            uint8_t bmp = Q_EVT_CAST(ObjectImageEvt)->bmp;
            // ${AOs::Mine2::SM::used::planted::MISSILE_IMG::[collisionwith~]}
            if (do_bitmaps_overlap(MINE2_MISSILE_BMP, me->m_x, me->m_y, bmp, x, y)) {
                static struct {
                    QP::QMState const *target;
                    QP::QActionHandler act[2];
                } const tatbl_ = { // transition-action table
                    &exploding_s,
                    {
                        Q_ACTION_CAST(&exploding_e), // entry
                        Q_ACTION_CAST(0)  // zero terminator
                    }
                };
                static ScoreEvt const mine2_destroyed(DESTROYED_MINE_SIG, 45U);
                AO_Missile->POST(&mine2_destroyed, me);
                status_ = QM_TRAN(&tatbl_);
            }
            else {
                status_ = QM_UNHANDLED();
            }
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}

} // namespace GAME