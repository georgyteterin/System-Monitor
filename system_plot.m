date = '12072023';
time = '154612';

% prompt_date = 'what date?\n';
% prompt_time = 'what time?\n';
% 
% date = input(prompt_date, "s");
% time = input(prompt_time, "s");



%cpu data 

filename1 = strcat('cpu_', date, '_', time, '.csv');
fileID1 = fopen(filename1);
CPU = textscan(fileID1,'%s %f %f', 'Delimiter',';','EmptyValue',-Inf);
fclose(fileID1);
max_cpu = max(CPU{2});
min_cpu = min(CPU{2});
axx = datenum(CPU{1}, 'HH:MM:SS');

% ram data

filename2 = strcat('ram_', date, '_', time, '.csv');
fileID2 = fopen(filename2);
RAM = textscan(fileID2,'%s %f %f', 'Delimiter',';','EmptyValue',-Inf);
fclose(fileID2);
byy = datenum(RAM{1}, 'HH:MM:SS');

tiledlayout(2, 1);

% plotting cpu
nexttile;
cpu_plot = plot(axx, CPU{2});
grid on;
xlabel('time');
ylabel('CPU, %');


ylim([min_cpu - 2, max_cpu + 2]);
datetick(gca);

% plotting ram

nexttile;
ram_plot = plot(byy, RAM{2});
grid on;
xlabel('time');
ylabel('RAM, Mb');
ylim([0, 4096]);
datetick(gca);

