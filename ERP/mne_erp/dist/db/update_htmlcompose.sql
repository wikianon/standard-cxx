alter table mne_application.userpref DROP CONSTRAINT userpref_startweblet_fkey; 
alter table mne_application.htmlcomposetabslider DROP CONSTRAINT htmlcomposetabslider_pkey;

alter table mne_application.htmlcomposetabselect DROP CONSTRAINT htmlcomposetabselect_pkey;
alter table mne_application.htmlcomposetabselect DROP CONSTRAINT htmlcomposetabselect_name_fkey;

alter table mne_application.htmlcomposetabnames DROP CONSTRAINT htmlcomposetabnames_pkey;
alter table mne_application.htmlcomposetabnames DROP CONSTRAINT htmlcomposetabnames_name_fkey;

alter table mne_application.htmlcomposetab DROP CONSTRAINT htmlcomposetab_pkey;
alter table mne_application.htmlcomposetab DROP CONSTRAINT htmlcomposetab_name_fkey;

alter table mne_application.htmlcomposenames DROP CONSTRAINT htmlcomposenames_pkey;
alter table mne_application.htmlcomposenames DROP CONSTRAINT htmlcomposenames_name_fkey;

alter table mne_application.htmlcompose DROP CONSTRAINT htmlcompose_pkey;

alter table mne_application.htmlcompose add column htmlcomposeid VARCHAR(32);
update mne_application.htmlcompose set htmlcomposeid = mne_catalog.mk_id() WHERE htmlcomposeid IS NULL;
alter table mne_application.htmlcompose alter column htmlcomposeid DROP DEFAULT;
alter table mne_application.htmlcompose alter column htmlcomposeid SET NOT NULL;

alter table mne_application.htmlcomposenames add column htmlcomposeid VARCHAR(32);
update mne_application.htmlcomposenames t0 set htmlcomposeid = ( select htmlcomposeid FROM mne_application.htmlcompose t1 WHERE t0.name = t1.name );
alter table mne_application.htmlcomposenames alter column htmlcomposeid DROP DEFAULT;
alter table mne_application.htmlcomposenames alter column htmlcomposeid SET NOT NULL;

alter table mne_application.htmlcomposenames alter column name DROP NOT NULL;


alter table mne_application.htmlcomposetab add column htmlcomposeid VARCHAR(32);
update mne_application.htmlcomposetab t0 set htmlcomposeid = ( select htmlcomposeid FROM mne_application.htmlcompose t1 WHERE t0.name = t1.name );
alter table mne_application.htmlcomposetab alter column htmlcomposeid DROP DEFAULT;
alter table mne_application.htmlcomposetab alter column htmlcomposeid SET NOT NULL;

alter table mne_application.htmlcomposetab add column htmlcomposetabid VARCHAR(32);
update mne_application.htmlcomposetab set htmlcomposetabid = mne_catalog.mk_id() WHERE htmlcomposetabid IS NULL;
alter table mne_application.htmlcomposetab alter column htmlcomposetabid DROP DEFAULT;
alter table mne_application.htmlcomposetab alter column htmlcomposetabid SET NOT NULL;

alter table mne_application.htmlcomposetab alter column name DROP NOT NULL;


alter table mne_application.htmlcomposetabnames add column htmlcomposeid VARCHAR(32);
update mne_application.htmlcomposetabnames t0 set htmlcomposeid = ( select htmlcomposeid FROM mne_application.htmlcompose t1 WHERE t0.name = t1.name );
alter table mne_application.htmlcomposetabnames alter column htmlcomposeid DROP DEFAULT;
alter table mne_application.htmlcomposetabnames alter column htmlcomposeid SET NOT NULL;

alter table mne_application.htmlcomposetabnames add column htmlcomposetabid VARCHAR(32);
update mne_application.htmlcomposetabnames t0 set htmlcomposetabid = ( select htmlcomposetabid FROM mne_application.htmlcomposetab t1 WHERE t0.name = t1.name and t0.id = t1.id);
alter table mne_application.htmlcomposetabnames alter column htmlcomposetabid DROP DEFAULT;
alter table mne_application.htmlcomposetabnames alter column htmlcomposetabid SET NOT NULL;

alter table mne_application.htmlcomposetabnames alter column name DROP NOT NULL;
alter table mne_application.htmlcomposetabnames alter column id DROP NOT NULL;


alter table mne_application.htmlcomposetabselect add column htmlcomposeid VARCHAR(32);
update mne_application.htmlcomposetabselect t0 set htmlcomposeid = ( select htmlcomposeid FROM mne_application.htmlcompose t1 WHERE t0.name = t1.name );
alter table mne_application.htmlcomposetabselect alter column htmlcomposeid DROP DEFAULT;
alter table mne_application.htmlcomposetabselect alter column htmlcomposeid SET NOT NULL;

alter table mne_application.htmlcomposetabselect add column htmlcomposetabid VARCHAR(32);
update mne_application.htmlcomposetabselect t0 set htmlcomposetabid = ( select htmlcomposetabid FROM mne_application.htmlcomposetab t1 WHERE t0.name = t1.name and t0.id = t1.id);
alter table mne_application.htmlcomposetabselect alter column htmlcomposeid DROP DEFAULT;
alter table mne_application.htmlcomposetabselect alter column htmlcomposeid SET NOT NULL;

alter table mne_application.htmlcomposetabselect add column htmlcomposetabselectid VARCHAR(32);
update mne_application.htmlcomposetabselect set htmlcomposetabselectid = mne_catalog.mk_id() WHERE htmlcomposetabselectid IS NULL;
alter table mne_application.htmlcomposetabselect alter column htmlcomposetabselectid DROP DEFAULT;
alter table mne_application.htmlcomposetabselect alter column htmlcomposetabselectid SET NOT NULL;

alter table mne_application.htmlcomposetabselect alter column name DROP NOT NULL;


alter table mne_application.htmlcomposetabslider alter column name DROP NOT NULL;
alter table mne_application.htmlcomposetabslider add column htmlcomposeid VARCHAR(32);
update mne_application.htmlcomposetabslider t0 set htmlcomposeid = ( select htmlcomposeid FROM mne_application.htmlcompose t1 WHERE t0.name = t1.name );
alter table mne_application.htmlcomposetabslider alter column htmlcomposeid DROP DEFAULT;
alter table mne_application.htmlcomposetabslider alter column htmlcomposeid SET NOT NULL;

alter table mne_application.htmlcomposetabslider alter column name DROP NOT NULL;

ALTER TABLE mne_application.htmlcomposetabnames DROP COLUMN id;
ALTER TABLE mne_application.htmlcomposetabnames DROP COLUMN name;
ALTER TABLE mne_application.htmlcomposetabselect DROP COLUMN name;
ALTER TABLE mne_application.htmlcomposetabslider DROP COLUMN name;
ALTER TABLE mne_application.htmlcomposenames DROP COLUMN name;
ALTER TABLE mne_application.htmlcomposetab DROP COLUMN name;

