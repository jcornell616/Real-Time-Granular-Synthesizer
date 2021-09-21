%% AUDIO IN %%
[audio fs] = audioread('Ravel.wav');
IN = audio(1:fs*60,1);
%soundsc(IN,fs);
in_size = size(IN,1);

%% USER_DEFINED GRANULAR PARAMETERS %%
% GRAIN SIZE: length of each grain in milliseconds, 5 to 500 ms
g_size = 80;
% GRAIN TEXTURE: related to grain density, between 0.2 to 1.8
g_texture = 1.0;
% WET: ratio of grains volume to input volume, 0 to 1
wet = 0.5;
% FDBK: grain signal written back to buffer, 0 to 1
%not implemented for MATLAB simulation
% VOL: input volume, 0 to 2
vol = 1.0;
% FREEZE: determines if input signal written to buffer, boolean value
frozen = 0;
% RANDOM: determines if grains generated synchronously or asynchronously, boolean value
random = 0;
% REVERSE: determines if grains played in forward or reverse, boolean value
reverse = 0;
% ENVELOPE SELECTION: chooses between parabolic and cosine bell envelope
env_sel = 1;

%% COMPUTED GRANULAR PARAMETERS %%
% GRAIN size (SAMPLES): grain size in samples
g_size_s = floor(fs*g_size/1000);
% GRAIN COUNT: number of grains to generate, not necessary in real-time
g_cnt = floor(in_size/g_size_s);
% ENVELOPE: envelope applied to each grain
% Raised cosine bell, only generate attack portion, sustain portion is
% normal grain amplitude, decay is inverse of attack
if env_sel==0
    env_size = floor(g_size_s/40);
    if env_size <= 20
        env_size = 20;
    end
else
    env_size = g_size_s;
end
ENVELOPE = zeros(env_size,1);
ENV_PLOT = 1:env_size;
for i=1:env_size
    if env_sel==0
        ENVELOPE(i) = 1.0 + cos(pi+(pi*(i/env_size))*(1.0/2.0));
    else
        amp = 0;
        rdur = 1.0/g_size_s;
        rdur2 = rdur*rdur;
        slope = 4.0*(rdur-rdur2);
        curve = -8.0*rdur2;
        for i = 1:env_size
            ENVELOPE(i) = amp;
            amp = amp+slope;
            slope = slope+curve;
        end
    end
end;
figure(1);
plot(ENV_PLOT, ENVELOPE);
title('Envelope');

%% GENERATE GRAINS %%
%generate 1 grain for figure
grain = zeros(g_size_s,1);
grain_plot = 1:g_size_s;
for i=1:g_size_s
    if i <= env_size
        grain(i) = ENVELOPE(i)*IN(i+floor(in_size/3));
    elseif i >= (g_size_s+1-env_size)
            grain(i) = ENVELOPE(g_size_s+1-i)*IN(i+floor(in_size/3));
    else
        grain(i) = IN(i+floor(in_size/3));
    end
end
%soundsc(grain,fs);
figure(2);
plot(grain_plot, grain);
title('Grain Waveform');
% generate all grains
% in real time, grains are generated in dynamically and simply pointers to
% first sample of grain. They will be managed in a linked list data structure.
grains = zeros(g_size_s,g_cnt);
for i = 1:g_cnt
    pos = ((i-1)*g_size_s)+1;
    grains(:,i) = generate_grain(IN,ENVELOPE,g_size_s,pos);
end
%% COMPUTATION %%
% calculate input portion
OUT = zeros(floor(2.0*size(IN,1)),1);
OUT(1:size(IN,1)) = vol*(1-wet).*IN;
%calculate grain portion
temp = 0;
%if frozen true, choose random grain to play. In real-time, grains being
%played when switch goes true will be chosen.
if frozen==1
    g_n = 1 + floor(rand()*(g_cnt-1));
    temp = 0;
     for i = 1:g_cnt
        for j = 1:g_size_s
            if reverse == 0
                OUT(temp+j) = (OUT(temp+j) + wet*grains(j,g_n))/2;
            else
                OUT(temp+j) = (OUT(temp+j) + wet*grains(g_size_s+1-j,g_n))/2;
            end
        end
        temp = temp+(g_texture*g_size_s);
     end
%otherwise, perform normal granulr synthesis
else
    for i = 1:g_cnt
        %determine hop size--the amount of samples from start of grain
        %until new grain is generated--either linearly or randomly
        if rand==1
            hop_size = floor(g_texture*g_size_s+(rand()*0.9*g_size_s));
        else
            hop_size = floor(g_size_s*g_texture);
        end
        temp = temp + hop_size;
        if random==1
            g_n = floor(1+g_cnt*rand());
        else
            g_n = i;
        end
        %write grain to audio
        for j = 1:g_size_s
            if reverse == 0
                OUT(temp+j) = (OUT(temp+j) + wet*grains(j,g_n))/2;
            else
                OUT(temp+j) = (OUT(temp+j) + wet*grains(g_size_s+1-j,g_n))/2;
            end
        end
    end
end

%% AUDIO OUT %%
seconds = 20;
soundsc(OUT(1:fs*seconds),fs);
figure(3);
plot(fs*2:floor(fs*2.2),OUT(fs*2:floor(fs*2.2)));
title('Output Waveform');