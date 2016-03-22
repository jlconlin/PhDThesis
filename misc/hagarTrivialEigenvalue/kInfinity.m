function [kHistory] = kInfinity(historiesPerCycle, cycles, SigmaS, SigmaG, SigmaF,...
                                nu, method)
% [kHistory] = kInfinity(historiesPerCycle, cycles, SigmaS, SigmaG, SigmaF, nu)
%
% Estimate k_\inf using a Monte Carlo
% historiesPerCycle - number of histories to sample from the
%                     fission source during each cycle
% cycles - number of cycles to run
% SigmaS, SigmaG, SigmaF - Macroscopic cross sections for
%                          scatter, radiative capture, fission
% nu - mean number of fission neutrons born per fission
% method - a flag to select a method to use
%
% kHistory - the history of k values
%
% There are no dead cycles because the system is an infinite
% medium, so we don't need to get a critical fission source
% shape set up
%
% $Id$

validateData(historiesPerCycle, cycles, SigmaS, SigmaG, SigmaF, nu, method)

% ===========================================================
% Initialization of data
% ===========================================================

% Set up number of particles in previous bank.  We don't need
% to store phase space data for individual events becasue we 
% don't care where the particles were created
fissionSource = 1;  % initial source

% Compute the absorption and total cross sections
SigmaA = SigmaG + SigmaF;
SigmaT = SigmaS + SigmaA;

% Initialize the history of k values
kHistory = zeros(1,cycles+1);
kHistory(1) = rand()+0.0001;  % Not zero, but don't care what

for i = 2:length(kHistory)
   kHistory(i) = ... 
         computeK(kHistory(i-1), historiesPerCycle, SigmaS, SigmaG, SigmaF, nu, SigmaT, SigmaA, method);
   % disp([num2str(i-1) ': ' num2str(kHistory(i))])
   if (kHistory(i) <= 0)
       disp(['k negative or zero on cycle ' num2str(i-1)])
       break
   end
end

% End of kInfinity

% ===========================================================
% ===>> computeK <<==========================================
% ===========================================================
function k = computeK(kold, histories, SigmaS, SigmaG, SigmaF, nu, SigmaT, SigmaA, method)
% Dispatch the k computation to one of several methods.  Oh, for a decent
% OOP system!

if method == 1
  k = kMethod1(kold, histories, SigmaS, SigmaG, SigmaF, nu, SigmaT, SigmaA, method);
elseif method == 2
  k = kMethod2(kold, histories, SigmaS, SigmaG, SigmaF, nu, SigmaT, SigmaA, method);
else
  error(['Method number ', num2str(method), ' is invalid'])
end

% ===========================================================
% ===>> kMethod1 <<==========================================
% ===========================================================
function [k, newFissionSource] = kMethod1(kold, histories, SigmaS, SigmaG, SigmaF, ...
                                          nu, SigmaT, SigmaA, method)

% Sample histories particles from the fissionSource.  Note that
% since we are infinite dimensional the old fissionSource is
% not really used for sampling, becasue we don't need any phase
% space data.  Each particle is followed until it is absorbed,
% at which point we either add particles to the newFissionSource
% or not, depending on the probability of fission capture.
newFissionSource = 0;

for i = 1:histories
    % Track particle to absorbtion - nothing really to do

    % Particles has been absorbed, was it fission or radiative capture?
    interaction = rand();
    if(interaction * SigmaA > SigmaG)
        % it was a fission
        newParticles = floor(nu/kold + rand());
        newFissionSource = newFissionSource + newParticles;
    else
        % it was an absorbtion
    end
end
if(newFissionSource == 0)
    disp('Warning: there were no fissions on this cycle')
end
k = kold * newFissionSource / histories;

% ===========================================================
% ===>> kMethod2 <<==========================================
% ===========================================================
function [k, newFissionSource] = kMethod2(kold, histories, SigmaS, SigmaG, SigmaF, ...
                                          nu, SigmaT, SigmaA, method)

% Sample histories particles from the fissionSource.  Note that
% since we are infinite dimensional the old fissionSource is
% not really used for sampling, becasue we don't need any phase
% space data.  Each particle is followed until it is absorbed,
% at which point we always add particles to the newFissionSource
newFissionSource = 0;

for i = 1:histories
    % Track particle to absorbtion - nothing really to do

    interaction = rand();
    newParticles = floor(nu/kold * SigmaF/SigmaA + rand());
    newFissionSource = newFissionSource + newParticles;
end
if(newFissionSource == 0)
    disp('Warning: there were no fissions on this cycle')
end
k = kold * newFissionSource / histories;

% ===========================================================
% ===>> validateData <<======================================
% ===========================================================
function validateData(historiesPerCycle, cycles, SigmaS, SigmaG, SigmaF, nu, method)

if(historiesPerCycle < 1)
    error('historiesPerCycle must be positive')
end
if(cycles < 0)
    error('cycles must be non-negative')
end
if(SigmaS < 0.0)
    error('SigmaS must be non-negative')
end
if(SigmaG < 0.0)
    error('SigmaS must be non-negative')
end
if(SigmaF < 0.0)
    error('SigmaS must be non-negative')
end
if(nu < 0.0)
    error('nu must be non-negative')
end
