ALTER ROLE admindav RENAME TO adminshare;

UPDATE mne_application.menu 
SET ugroup = 'erp' || ugroup 
WHERE ugroup != '' AND substring(ugroup, 1, 5) != 'admin' and substring(ugroup, 1, 3 ) != 'erp' and substring(ugroup, 1, 3 ) != 'mne' AND substring(ugroup, 1, 4) != 'show'; 

UPDATE mne_application.htmlcomposetab
SET ugroup = 'erp' || ugroup
WHERE ugroup != '' AND substring(ugroup, 1, 5) != 'admin' and substring(ugroup, 1, 3 ) != 'erp' and substring(ugroup, 1, 3 ) != 'mne' AND substring(ugroup, 1, 4) != 'show' 
