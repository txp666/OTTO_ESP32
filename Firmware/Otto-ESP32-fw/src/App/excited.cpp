#include "image.h"

LV_IMG_DECLARE(excited1_1)LV_IMG_DECLARE(excited1_2)LV_IMG_DECLARE(excited1_3)LV_IMG_DECLARE(excited1_4)
LV_IMG_DECLARE(excited1_5)LV_IMG_DECLARE(excited1_6)LV_IMG_DECLARE(excited1_7)LV_IMG_DECLARE(excited1_8)
LV_IMG_DECLARE(excited1_9)LV_IMG_DECLARE(excited1_10)LV_IMG_DECLARE(excited1_11)LV_IMG_DECLARE(excited1_12)
LV_IMG_DECLARE(excited1_13)LV_IMG_DECLARE(excited1_14)LV_IMG_DECLARE(excited1_15)LV_IMG_DECLARE(excited1_16)
LV_IMG_DECLARE(excited1_17)LV_IMG_DECLARE(excited1_18)LV_IMG_DECLARE(excited1_19)LV_IMG_DECLARE(excited1_20)
LV_IMG_DECLARE(excited1_21)LV_IMG_DECLARE(excited1_22)LV_IMG_DECLARE(excited1_23)LV_IMG_DECLARE(excited1_24)
LV_IMG_DECLARE(excited1_25)LV_IMG_DECLARE(excited1_26)LV_IMG_DECLARE(excited1_27)LV_IMG_DECLARE(excited1_28)
LV_IMG_DECLARE(excited1_29)LV_IMG_DECLARE(excited1_30)LV_IMG_DECLARE(excited1_31)LV_IMG_DECLARE(excited1_32)
LV_IMG_DECLARE(excited1_33)LV_IMG_DECLARE(excited1_34)LV_IMG_DECLARE(excited1_35)LV_IMG_DECLARE(excited1_36)
LV_IMG_DECLARE(excited1_37)LV_IMG_DECLARE(excited1_38)LV_IMG_DECLARE(excited1_39)LV_IMG_DECLARE(excited1_40)
LV_IMG_DECLARE(excited1_41)LV_IMG_DECLARE(excited1_42)LV_IMG_DECLARE(excited1_43)LV_IMG_DECLARE(excited1_44)
LV_IMG_DECLARE(excited1_45)LV_IMG_DECLARE(excited1_46)LV_IMG_DECLARE(excited1_47)LV_IMG_DECLARE(excited1_48)
LV_IMG_DECLARE(excited1_49)LV_IMG_DECLARE(excited1_50)LV_IMG_DECLARE(excited1_51)LV_IMG_DECLARE(excited1_52)
LV_IMG_DECLARE(excited1_53)LV_IMG_DECLARE(excited1_54)LV_IMG_DECLARE(excited1_55)LV_IMG_DECLARE(excited1_56)
static const lv_img_dsc_t* excited1_anim_imgs[56] = {
&excited1_1,&excited1_2,&excited1_3,&excited1_4,
&excited1_5,&excited1_6,&excited1_7,&excited1_8,
&excited1_9,&excited1_10,&excited1_11,&excited1_12,
&excited1_13,&excited1_14,&excited1_15,&excited1_16,
&excited1_17,&excited1_18,&excited1_19,&excited1_20,
&excited1_21,&excited1_22,&excited1_23,&excited1_24,
&excited1_25,&excited1_26,&excited1_27,&excited1_28,
&excited1_29,&excited1_30,&excited1_31,&excited1_32,
&excited1_33,&excited1_34,&excited1_35,&excited1_36,
&excited1_37,&excited1_38,&excited1_39,&excited1_40,
&excited1_41,&excited1_42,&excited1_43,&excited1_44,
&excited1_45,&excited1_46,&excited1_47,&excited1_48,
&excited1_49,&excited1_50,&excited1_51,&excited1_52,
&excited1_53,&excited1_54,&excited1_55,&excited1_56,
};
void excited1_animing(void)
{
    lv_obj_t* animimg1 = lv_animimg_create(lv_scr_act());    //创建动画对象
    lv_obj_center(animimg1);                                 //将对象置于屏幕中央
    lv_animimg_set_src(animimg1,(lv_img_dsc_t **)excited1_anim_imgs,56);    //加载动画资源
    lv_animimg_set_duration(animimg1,2000);                   //创建动画时间
    lv_animimg_set_repeat_count(animimg1,LV_ANIM_REPEAT_INFINITE);               //设置一直重复时间
    lv_animimg_start(animimg1);
}