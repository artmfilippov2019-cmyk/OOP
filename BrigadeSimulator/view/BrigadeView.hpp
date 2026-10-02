#ifndef LAB3_DIALOGVIEW_H
#define LAB3_DIALOGVIEW_H

#include "IBrigadeView.hpp"
#include "presenter/IBrigadePresenter.hpp"

class BrigadeView : public IBrigadeView {
	IBrigadePresenter* presenter;

	static void show_menu();
	void menu_add_person() const;
	void menu_promote() const;
	void menu_add_worksite() const;
	void menu_assign() const;
	void menu_show_all() const;
	void menu_simulate() const;
	void menu_save() const;
	void menu_load() const;
	void menu_remove_person() const;
	void menu_get_person() const;
	void menu_change_worker_productivity() const;
	void menu_change_master_efficiency() const;
	void menu_add_friendship() const;
	void menu_add_enmity() const;
	void menu_remove_friendship() const;
	void menu_remove_enmity() const;
	void menu_change_worksite_volume() const;
	void menu_remove_person_from_site() const;
	void menu_get_worksite() const;
	void menu_get_all_worksites() const;
	void menu_calculate_site_output() const;
	void menu_check_all_completed() const;
	void menu_get_most_problematic() const;
	void menu_timing() const;

public:
	explicit BrigadeView(IBrigadePresenter* p);

	void set_presenter(IBrigadePresenter* p);

	void run() override;
};

#endif