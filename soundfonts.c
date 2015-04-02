//
//  soundfonts.c
//
//
//  Created by Chris Penny on 2/23/15.
//  Copyright (c) 2015 Comp150. All rights reserved.
//

#include "m_pd.h"

static t_class *soundfonts_class;

typedef struct _soundfonts {
    t_object x_obj;
    
    // Outlets
    t_outlet *x_outL;
    t_outlet *x_outR;
    
} t_soundfonts;

void *soundfonts_new(void)
{
    // Get Class Instance
    t_soundfonts *instance = (t_soundfonts *)pd_new(soundfonts_class);

    // Spawn outlets
    instance->x_outL = outlet_new(&instance->x_obj, &s_signal);
    instance->x_outR = outlet_new(&instance->x_obj, &s_signal);
    
    return (void *)instance;
}

void soundfonts_free(t_soundfonts *instance)
{

    outlet_free(instance->x_outL);
    outlet_free(instance->x_outR);
}

t_int *soundfonts_perform(t_int *input)
{
    t_sample *outL = (t_sample *)(input[2]);
    t_sample *outR = (t_sample *)(input[3]);
    
    for(int i = 0; i < (int)(input[4]); i++) {
        outL[i] = 14;
        outR[i] = 28;
    }
    
    return input + 5;
}

void soundfonts_dsp(t_soundfonts *instance, t_signal **sp)
{
    dsp_add(soundfonts_perform, 4, instance,
            sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

void soundfonts_info(t_soundfonts *instance)
{
    post("soundfonts version 0.0");
}

void soundfonts_setup(void)
{
    soundfonts_class = class_new(gensym("soundfonts"),
                                       (t_newmethod)soundfonts_new,
                                       0,
                                       sizeof(t_soundfonts),
                                       CLASS_DEFAULT,
                                       0);
    
    class_addmethod(soundfonts_class, (t_method)soundfonts_dsp, gensym("dsp"), 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_info, gensym("info"), 0);
}

