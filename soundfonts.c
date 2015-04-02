//
//  soundfonts.c
//
//
//  Created by Chris Penny on 2/23/15.
//  Copyright (c) 2015 Comp150. All rights reserved.
//

#include "m_pd.h"
#include "fluidsynth.h"

static t_class *soundfonts_class;

typedef struct _soundfonts {
    t_object x_obj;
    
    fluid_settings_t *settings;
    fluid_synth_t *synth;
    
    t_int sfont_id;
    
    // Outlets
    t_outlet *x_outL;
    t_outlet *x_outR;
    
} t_soundfonts;

void *soundfonts_new(void)
{
    // Get Class Instance
    t_soundfonts *instance = (t_soundfonts *)pd_new(soundfonts_class);

    instance->settings = new_fluid_settings();
    instance->synth = new_fluid_synth(instance->settings);
    instance->sfont_id = fluid_synth_sfload(instance->synth, "/Users/ChrisPenny/Desktop/Comp150/Pd-for-LibPd/soundfonts/piano_1.sf2", 1);
    
    // Spawn outlets
    instance->x_outL = outlet_new(&instance->x_obj, &s_signal);
    instance->x_outR = outlet_new(&instance->x_obj, &s_signal);
    
    return (void *)instance;
}

void soundfonts_free(t_soundfonts *instance)
{
    delete_fluid_settings(instance->settings);
    delete_fluid_synth(instance->synth);

    outlet_free(instance->x_outL);
    outlet_free(instance->x_outR);
}

t_int *soundfonts_perform(t_int *input)
{
    t_soundfonts *instance = (t_soundfonts *)(input[1]);
    t_sample *outL = (t_sample *)(input[2]);
    t_sample *outR = (t_sample *)(input[3]);
    int n = (int)(input[4]);
    
    for(int i = 0; i < n; i++) {
        fluid_synth_write_float(instance->synth, n, outL, 0, 1, outR, 0, 1);
    }
    
    return input + 5;
}

void soundfonts_dsp(t_soundfonts *instance, t_signal **sp)
{
    dsp_add(soundfonts_perform, 4, instance,
            sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

void soundfonts_play(t_soundfonts *instance)
{
    fluid_synth_noteon(instance->synth, 0, 60, 127);
}

void soundfonts_stop(t_soundfonts *instance)
{
    fluid_synth_noteoff(instance->synth, 0, 60);
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
    class_addmethod(soundfonts_class, (t_method)soundfonts_play, gensym("play"), 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_stop, gensym("stop"), 0);
}

