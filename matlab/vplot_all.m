function [] = vplot_all(var)

  for idx = 1 : numel(var)
    subplot(numel(var),1,idx)
    stairs(var(idx).time,var(idx).value,"-x-");
    ylabel(var(idx).name,'Interpreter','none');
    grid on;
  end

end