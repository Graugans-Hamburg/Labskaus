function [] = vplot(varnumber, var)

  stairs(var(varnumber).time,var(varnumber).value);
  ylabel(var(varnumber).name,'Interpreter','none');
  grid on;

  end