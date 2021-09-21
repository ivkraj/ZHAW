figure(1);
clf;

clear;
clc;
x = zeros(1,28);
y = zeros(1,28);
x(1) = 10;
y(1) = 0;
y(28)= 100;
x(28)= 40;

for i = 2:27
    y(i) = 100/25*(i-2);
    x(i) = 20+(10/25)*(i-2);
end

stairs(x, y, 'r', 'LineWidth', 2);
hold on;
stairs(x, (100-y), 'b', 'LineWidth', 2);
grid on;
xlabel('temperature in °C');
ylabel('component value in %');
legend('blue component', 'red component','Location', 'East');

mymap = [0 0 1
         0 0 1
         0 0 1
         0 0 1
         0 0 1
         0 0 1
         0 0 1
         0 0 1
         0 0 1
         0 0 1
         0 0 1
         0 0 1
         0 0 1
         0 0 1
         0 0 1
         0 0 1
         0 0 1
         0 0 1
         0 0 1
         0 0 1
         0.04 0 0.96
         0.08 0 0.92
         0.12 0 0.88
         0.16 0 0.84
         0.20 0 0.80
         0.24 0 0.76
         0.28 0 0.72
         0.32 0 0.68
         0.36 0 0.64
         0.40 0 0.60
         0.44 0 0.56
         0.48 0 0.52
         0.52 0 0.48
         0.56 0 0.44
         0.60 0 0.40
         0.64 0 0.36
         0.68 0 0.32
         0.72 0 0.28
         0.76 0 0.24
         0.80 0 0.20
         0.84 0 0.16
         0.88 0 0.12
         0.92 0 0.08
         0.96 0 0.04
         1 0 0
         1 0 0
         1 0 0
         1 0 0
         1 0 0
         1 0 0
         1 0 0
         1 0 0
         1 0 0
         1 0 0
         1 0 0
         1 0 0
         1 0 0
         1 0 0
         1 0 0
         1 0 0
         1 0 0
         1 0 0
         1 0 0
         1 0 0];
colormap(mymap);

caxis([10 40]);
colorbar1 = colorbar(...
    'northoutside',...
    'FontSize',10,...
    'XLim',[10 40]);
colorbar1.Label.String = 'Moodlight Colour';

figure(2);
clf;
%stairs(x, y, 'r', 'LineWidth', 2);
colormap(jet);
caxis([10 40]);
colorbar2 = colorbar(...
    'northoutside',...
    'FontSize',10,...
    'XLim',[10 40]);

figure(3);
clf;
mymap2 = [1 0 1
         0 0.7 1
         0.8 0 0.9
         0.32 0.5 0.7
         0.4 0.6 0.1
         0.2 0.5 0.6
         0.0 1 0.9 
         0.5 0.8 0.37
         0.92 0.64 0.21
         0.45 0.79 0.66
         0.35 0.12 0
         0.79 0.02 0.2
         0.19 0.02 0.5
         1 1 1];
colormap(mymap2);
colorbar3 = colorbar(...
    'northoutside',...
    'FontSize',10);

