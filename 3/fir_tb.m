function fir_tb

    %------------------------------------------------------------
    % Note: it appears that the cosimWizard needs to be re-run if
    % this is moved to a different machine (VHDL needs to be
    % recompile in ModelSim).
    %------------------------------------------------------------

    % HdlCosimulation System Object creation (this Matlab function was created
    % by the cosimWizard).
    fir_hdl = hdlcosim_fir;

    % Simulate for Nclock rising edges (this will be the length of the
    % simulation)
    Nclock_edges = 32769;

    for clki=1:Nclock_edges
        %-----------------------------------------------------------------
        % Create our input vector at each clock edge, which must be a
        % fixed-point data type. The word width of the fixed point data type
        % must match the width of the std_logic_vector input.
        %-----------------------------------------------------------------
        fixed_word_width = 16; % width of input to component
        fixed_point_signed = 1; % unsigned = 0, signed = 1;
        fixed_point_fraction = 0; % fraction width (location of binary point within word)
        fixed_point_value = clki; % choose a random input values over an appropriate range
        input_vector1 = fi(fixed_point_value, fixed_point_signed, fixed_word_width, fixed_point_fraction); % make the input a fixed point data type
        input_history{clki} = input_vector1; % capture the inputs

        %-----------------------------------------------------------------
        % Push the input(s) into the component using the step function on the
        % system object fir_hdl
        % If there are multiple I/O, use
        % [out1, out2, out3] = step(fir_hdl, in1, in2, in3);
        % and understand all I/O data types are fixed-point objects
        % where the inputs can be created by the fi() function.
        %-----------------------------------------------------------------
        output_vector1 = step(fir_hdl,input_vector1);

        %-----------------------------------------------------------------
        % Save the outputs (which are fixed-point objects)
        %-----------------------------------------------------------------
        output_history{clki} = output_vector1; % capture the output

    end
    %-----------------------------------------------------------------
    % Display the captured I/O
    %-----------------------------------------------------------------
    display_this = 0;
    if display_this == 1
        for clki=1:Nclock_edges
            in1 = input_history{clki};
            in1.bin
            out1 = output_history{clki}
            out1.dec
            out1.WordLength
        end
    end
    %-----------------------------------------------------------------
    % Perform the desired comparison (with the latency between input
    % and output appropriately corrected).
    %----------------------------------------------------------------- 
    latency = 4; % latency in clock cycles through the component
    error_index = 1;
    error_case = [];
    for clki=1:Nclock_edges-latency
        in1 = input_history{clki};
        out1 = output_history{clki+latency}; % get the output associated with current output
        in2 = input_history{clki+1};
        in3 = input_history{clki+2};
        
        
        trueout = fi((6*in2)+(5*in3)-(in1*(2)), 0, 34, 0);
        %------------------------------------------------------
        % Perfom the comparison with the "true" output
        %------------------------------------------------------
        
        
        if (output_history{clki+latency} == trueout)
            fprintf('Inputs are %d, %d, and %d\n', storedInteger(input_history{clki}), storedInteger(input_history{clki+1}), storedInteger(input_history{clki+2}))
            fprintf('%d = %d Passed\n', storedInteger(trueout), storedInteger(output_history{clki+latency}));
        else
            fprintf('%d != %d Failed\n', storedInteger(trueout), storedInteger(output_history{clki+latency}));
            pause
            
            
        end
            
            


    end
    
    %disp(input_history{5});
    %disp(output_history{5});
    

end