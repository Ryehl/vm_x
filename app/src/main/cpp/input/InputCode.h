//
// Created by Xaoyv on 2023/4/25.
//

#ifndef VM_INPUTCODE_H
#define VM_INPUTCODE_H

/*
 * 0x2e is reserved and should not be used in input drivers.
 * It was used by HID as ABS_MISC+6 and userspace needs to detect if
 * the next ABS_* event is correct or is just ABS_MISC + n.
 * We define here ABS_RESERVED so userspace can rely on it and detect
 * the situation described above.
 */
#define ABS_RESERVED		0x2e

#define ABS_MT_SLOT		0x2f	/* MT slot being modified */  //47
#define ABS_MT_TOUCH_MAJOR	0x30	/* Major axis of touching ellipse */  //48
#define ABS_MT_TOUCH_MINOR	0x31	/* Minor axis (omit if circular) */  //49
#define ABS_MT_WIDTH_MAJOR	0x32	/* Major axis of approaching ellipse */  //50
#define ABS_MT_WIDTH_MINOR	0x33	/* Minor axis (omit if circular) */  //51
#define ABS_MT_ORIENTATION	0x34	/* Ellipse orientation */  //52
#define ABS_MT_POSITION_X	0x35	/* Center X touch position */  //53
#define ABS_MT_POSITION_Y	0x36	/* Center Y touch position */  //54
#define ABS_MT_TOOL_TYPE	0x37	/* Type of touching device */  //55
#define ABS_MT_BLOB_ID		0x38	/* Group a set of packets as a blob */  //56
#define ABS_MT_TRACKING_ID	0x39	/* Unique ID of initiated contact */  //57
#define ABS_MT_PRESSURE		0x3a	/* Pressure on contact area */  //58
#define ABS_MT_DISTANCE		0x3b	/* Contact hover distance */  //59
#define ABS_MT_TOOL_X		0x3c	/* Center X tool position */  //60
#define ABS_MT_TOOL_Y		0x3d	/* Center Y tool position */  //61


#define ABS_MAX			0x3f
#define ABS_CNT			(ABS_MAX+1)


/*
 * Event types
 */

#define EV_SYN			0x00
#define EV_KEY			0x01
#define EV_REL			0x02
#define EV_ABS			0x03
#define EV_MSC			0x04
#define EV_SW			0x05
#define EV_LED			0x11
#define EV_SND			0x12
#define EV_REP			0x14
#define EV_FF			0x15
#define EV_PWR			0x16
#define EV_FF_STATUS		0x17
#define EV_MAX			0x1f
#define EV_CNT			(EV_MAX+1)

enum {
    ACTION_DOWN = 0,
    ACTION_UP = 1,
    ACTION_MOVE = 2,
    ACTION_CANCEL = 3,
    ACTION_POINTER_DOWN = 5,
    ACTION_POINTER_UP = 6,

    ACTION_MASK = 0xFF,
    ACTION_POINTER_INDEX_MASK = 0xFF00,
    ACTION_POINTER_INDEX_SHIFT = 8,
};

#endif //VM_INPUTCODE_H
