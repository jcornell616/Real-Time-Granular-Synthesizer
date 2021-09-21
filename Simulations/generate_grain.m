function [g] = generate_grain(audio_in,envelope,grain_size,pos)
% generate grain: creates a grain
%
% Input Args:
%   audio_in: waveform from which grain is being generated
%   envelope: attack portion of envelope
%   grain_size: size of grains in samples
%   pos: position in waveform from which grain starts
%
% Output:
%   g: grain waveform

g = zeros(grain_size,1);
env_size = size(envelope,1);
for i=1:grain_size
    if i <= env_size
        g(i) = envelope(i)*audio_in(i+pos-1);
    elseif i >= (grain_size+1-env_size)
            g(i) = envelope(grain_size+1-i)*audio_in(i+pos-1);
    else
        g(i) = audio_in(i+pos-1);
    end
end

