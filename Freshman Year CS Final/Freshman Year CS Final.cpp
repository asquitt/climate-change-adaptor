// Climate Change Adaptor - Comprehensive Sea Level Rise Approximation System
// Enhanced version with multiple scenarios, extensive country database, and detailed analysis

#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <algorithm>
using namespace std;

// Structure to hold country/region data
struct Region {
	string name;
	double areaSqKm;
	double coastlineKm;
	double avgElevation;
	double populationMillions;
	double coastalPopulationPercent;
	double gdpBillions;
	string vulnerability; // Low, Medium, High, Critical
};

// Sea level rise scenarios based on IPCC projections
struct SeaRiseScenario {
	string name;
	double riseRatePerYear; // cm per year
	double baseYear;
	double projectedRise2050; // cm
	double projectedRise2100; // cm
};

// Initialize sea level rise scenarios
vector<SeaRiseScenario> initializeScenarios() {
	vector<SeaRiseScenario> scenarios;

	// Conservative scenario (RCP 2.6 - strong mitigation)
	scenarios.push_back({"Conservative (RCP 2.6)", 0.32, 2025, 29.0, 43.0});

	// Moderate scenario (RCP 4.5 - intermediate)
	scenarios.push_back({"Moderate (RCP 4.5)", 0.44, 2025, 38.0, 63.0});

	// High scenario (RCP 6.0 - business as usual)
	scenarios.push_back({"High (RCP 6.0)", 0.52, 2025, 44.0, 73.0});

	// Extreme scenario (RCP 8.5 - worst case)
	scenarios.push_back({"Extreme (RCP 8.5)", 0.64, 2025, 52.0, 84.0});

	return scenarios;
}

// Initialize comprehensive region database
vector<Region> initializeRegions() {
	vector<Region> regions;

	// Island Nations (Most Vulnerable)
	regions.push_back({"Maldives", 298, 644, 1.5, 0.54, 100, 5.7, "Critical"});
	regions.push_back({"Tuvalu", 26, 24, 2.0, 0.012, 100, 0.047, "Critical"});
	regions.push_back({"Marshall Islands", 181, 370, 2.1, 0.059, 100, 0.24, "Critical"});
	regions.push_back({"Kiribati", 811, 1143, 2.0, 0.12, 100, 0.22, "Critical"});
	regions.push_back({"Seychelles", 455, 491, 3.0, 0.098, 95, 1.7, "Critical"});
	regions.push_back({"Palau", 459, 1519, 3.5, 0.018, 90, 0.31, "Critical"});
	regions.push_back({"Nauru", 21, 30, 3.0, 0.011, 100, 0.16, "Critical"});

	// Pacific Island Nations
	regions.push_back({"Solomon Islands", 28400, 5313, 15.0, 0.69, 85, 1.6, "High"});
	regions.push_back({"Fiji", 18272, 1129, 23.0, 0.90, 75, 5.5, "High"});
	regions.push_back({"Vanuatu", 12189, 2528, 25.0, 0.31, 80, 0.95, "High"});
	regions.push_back({"Micronesia", 702, 6112, 4.0, 0.11, 100, 0.40, "Critical"});
	regions.push_back({"Tonga", 747, 419, 5.0, 0.11, 95, 0.50, "High"});
	regions.push_back({"Samoa", 2842, 403, 8.0, 0.20, 90, 0.85, "High"});
	regions.push_back({"Cook Islands", 236, 120, 4.0, 0.017, 100, 0.38, "Critical"});
	regions.push_back({"French Polynesia", 4167, 2525, 6.0, 0.28, 95, 5.8, "High"});

	// Caribbean Nations
	regions.push_back({"Jamaica", 10991, 1022, 18.0, 2.96, 60, 16.5, "High"});
	regions.push_back({"Bahamas", 13878, 3542, 3.0, 0.39, 85, 12.8, "Critical"});
	regions.push_back({"Barbados", 430, 97, 5.0, 0.29, 100, 5.2, "High"});
	regions.push_back({"Trinidad and Tobago", 5128, 362, 10.0, 1.40, 45, 24.1, "Medium"});
	regions.push_back({"Cuba", 109884, 3735, 60.0, 11.33, 35, 107.4, "Medium"});
	regions.push_back({"Haiti", 27750, 1771, 145.0, 11.40, 30, 19.9, "High"});
	regions.push_back({"Dominican Republic", 48671, 1288, 80.0, 10.85, 40, 94.2, "Medium"});

	// Asian Coastal Nations
	regions.push_back({"Bangladesh", 148460, 580, 85.0, 166.3, 55, 416.3, "Critical"});
	regions.push_back({"Vietnam", 331212, 3444, 150.0, 97.3, 45, 366.1, "High"});
	regions.push_back({"Philippines", 300000, 36289, 140.0, 111.0, 50, 404.3, "High"});
	regions.push_back({"Indonesia", 1904569, 54716, 120.0, 273.5, 40, 1119.2, "High"});
	regions.push_back({"Thailand", 513120, 3219, 150.0, 69.8, 30, 505.9, "Medium"});
	regions.push_back({"Myanmar", 676578, 1930, 190.0, 54.4, 25, 76.1, "Medium"});
	regions.push_back({"Singapore", 728, 193, 15.0, 5.85, 100, 397.0, "High"});

	// Indian Ocean Region
	regions.push_back({"Sri Lanka", 65610, 1340, 100.0, 21.4, 45, 84.0, "High"});
	regions.push_back({"Mauritius", 2040, 177, 12.0, 1.27, 95, 14.2, "High"});
	regions.push_back({"Comoros", 2235, 340, 40.0, 0.87, 85, 1.3, "High"});
	regions.push_back({"Madagascar", 587041, 4828, 240.0, 28.0, 35, 14.1, "Medium"});

	// African Coastal Nations
	regions.push_back({"Gambia", 11295, 80, 34.0, 2.42, 70, 1.9, "High"});
	regions.push_back({"Senegal", 196722, 531, 69.0, 16.7, 40, 27.6, "Medium"});
	regions.push_back({"Nigeria", 923768, 853, 230.0, 206.1, 25, 440.8, "Medium"});
	regions.push_back({"Egypt", 1001450, 2450, 170.0, 102.3, 30, 404.1, "High"});
	regions.push_back({"Mozambique", 799380, 2470, 120.0, 31.3, 35, 15.1, "High"});

	// Major Economic Powers
	regions.push_back({"United States", 9833517, 19924, 760.0, 331.9, 25, 25346.8, "Medium"});
	regions.push_back({"China", 9596961, 14500, 1840.0, 1439.3, 20, 17734.1, "Medium"});
	regions.push_back({"Japan", 377975, 29751, 438.0, 125.8, 35, 4937.4, "High"});
	regions.push_back({"India", 3287263, 7517, 460.0, 1380.0, 20, 3173.4, "Medium"});
	regions.push_back({"United Kingdom", 242495, 12429, 162.0, 67.9, 30, 3124.7, "Medium"});
	regions.push_back({"Netherlands", 41543, 451, 30.5, 17.4, 60, 1012.8, "High"});
	regions.push_back({"Denmark", 42933, 7314, 34.0, 5.8, 45, 395.9, "High"});

	// US Cities/Regions
	regions.push_back({"Washington DC", 177, 0, 125.0, 0.71, 0, 141.0, "Medium"});
	regions.push_back({"Miami-Dade County", 6000, 200, 1.8, 2.7, 90, 160.0, "Critical"});
	regions.push_back({"New Orleans", 906, 85, 0.5, 0.39, 100, 95.0, "Critical"});
	regions.push_back({"New York City", 783, 837, 10.0, 8.3, 100, 1700.0, "High"});

	// South America
	regions.push_back({"Guyana", 214969, 459, 207.0, 0.79, 80, 6.8, "High"});
	regions.push_back({"Suriname", 163820, 386, 246.0, 0.59, 75, 3.6, "High"});
	regions.push_back({"French Guiana", 83534, 378, 150.0, 0.30, 70, 4.9, "Medium"});

	// Australia and Oceania
	regions.push_back({"Australia", 7692024, 25760, 330.0, 25.7, 25, 1392.7, "Medium"});
	regions.push_back({"New Zealand", 268838, 15134, 388.0, 5.1, 35, 242.8, "Medium"});

	return regions;
}

// Calculate sea level rise for a given year and scenario
double calculateSeaRise(int targetYear, const SeaRiseScenario& scenario) {
	if (targetYear <= scenario.baseYear) {
		return 0.0;
	}

	int yearsDiff = targetYear - (int)scenario.baseYear;

	// Use non-linear projection for better accuracy
	if (targetYear <= 2050) {
		double progress = (double)(targetYear - scenario.baseYear) / (2050 - scenario.baseYear);
		return scenario.projectedRise2050 * progress;
	} else if (targetYear <= 2100) {
		double progress = (double)(targetYear - 2050) / (2100 - 2050);
		return scenario.projectedRise2050 + (scenario.projectedRise2100 - scenario.projectedRise2050) * progress;
	} else {
		// Extrapolate beyond 2100 with increased rate
		int yearsAfter2100 = targetYear - 2100;
		return scenario.projectedRise2100 + (yearsAfter2100 * scenario.riseRatePerYear * 1.2);
	}
}

// Calculate ice melt impact for a region
void calculateIceMeltImpact(const Region& region) {
	const double annualIceMeltVolume = 16400.0; // cubic km per year

	double heightSubmerged = annualIceMeltVolume / region.areaSqKm;
	double metersEquivalent = heightSubmerged * 1000;
	double feetEquivalent = metersEquivalent * 3.28084;

	cout << "\n--- Ice Melt Impact Analysis for " << region.name << " ---" << endl;
	cout << "Annual ice melt volume distributed over " << region.name << ":" << endl;
	cout << "  Height: " << fixed << setprecision(2) << metersEquivalent << " meters or "
	     << feetEquivalent << " feet" << endl;
	cout << "  Coastline affected: " << region.coastlineKm << " km" << endl;
	cout << "  Vulnerability level: " << region.vulnerability << endl;
}

// Calculate population and economic impacts
void calculateImpacts(const Region& region, double seaLevelRiseCm) {
	double seaLevelRiseMeters = seaLevelRiseCm / 100.0;

	// Estimate affected population (simplified model)
	double elevationFactor = min(1.0, seaLevelRiseMeters / region.avgElevation);
	double coastalExposureFactor = region.coastalPopulationPercent / 100.0;
	double populationAtRisk = region.populationMillions * coastalExposureFactor * elevationFactor;

	// Estimate economic impact
	double economicImpact = region.gdpBillions * coastalExposureFactor * elevationFactor * 0.15;

	// Estimate land loss
	double landLossPercent = (seaLevelRiseMeters / region.avgElevation) * 100.0 * coastalExposureFactor;
	landLossPercent = min(landLossPercent, 100.0);
	double landLossArea = region.areaSqKm * (landLossPercent / 100.0);

	cout << "\n--- Impact Assessment for " << region.name << " ---" << endl;
	cout << "  Sea level rise: " << fixed << setprecision(2) << seaLevelRiseCm << " cm" << endl;
	cout << "  Population at risk: " << setprecision(2) << populationAtRisk << " million people" << endl;
	cout << "  Estimated economic impact: $" << setprecision(1) << economicImpact << " billion" << endl;
	cout << "  Potential land loss: " << setprecision(1) << landLossArea << " sq km ("
	     << setprecision(1) << landLossPercent << "%)" << endl;

	// Risk level assessment
	if (landLossPercent > 50 || region.vulnerability == "Critical") {
		cout << "  \n  *** SEVERE THREAT: Major territorial and population displacement expected ***" << endl;
	} else if (landLossPercent > 20 || region.vulnerability == "High") {
		cout << "  \n  ** HIGH RISK: Significant adaptation measures required **" << endl;
	} else if (landLossPercent > 5 || region.vulnerability == "Medium") {
		cout << "  \n  * MODERATE RISK: Coastal protection infrastructure needed *" << endl;
	} else {
		cout << "  \n  LOW RISK: Localized impacts expected" << endl;
	}
}

// Get islands at risk for a given year
void getIslandsAtRisk(int year) {
	cout << "\n=== Island Nations at Risk by " << year << " ===" << endl;

	if (year < 2030) {
		cout << "\nCRITICAL RISK (Partial submersion likely):" << endl;
		cout << "  - Maldives, Tuvalu, Marshall Islands, Kiribati" << endl;
		cout << "\nHIGH RISK (Severe flooding):" << endl;
		cout << "  - Solomon Islands, Palau, Nauru, Cook Islands" << endl;
	}
	else if (year < 2050) {
		cout << "\nCRITICAL RISK (Major submersion expected):" << endl;
		cout << "  - Maldives, Tuvalu, Marshall Islands, Kiribati, Nauru" << endl;
		cout << "  - Cook Islands, Micronesia (parts)" << endl;
		cout << "\nHIGH RISK (Extensive flooding and displacement):" << endl;
		cout << "  - Solomon Islands, Fiji, Seychelles, Palau, Vanuatu" << endl;
		cout << "  - Tonga, Samoa, French Polynesia (atolls)" << endl;
		cout << "\nMEDIUM RISK (Coastal areas severely affected):" << endl;
		cout << "  - Bahamas, Miami-Dade County, New Orleans, Bangladesh" << endl;
	}
	else if (year < 2075) {
		cout << "\nCRITICAL RISK (Likely uninhabitable):" << endl;
		cout << "  - Maldives, Tuvalu, Marshall Islands, Kiribati, Nauru" << endl;
		cout << "  - Cook Islands, Tokelau, portions of Micronesia" << endl;
		cout << "\nHIGH RISK (Major population relocation required):" << endl;
		cout << "  - Solomon Islands, Fiji, Vanuatu, Tonga, Samoa" << endl;
		cout << "  - French Polynesia (atolls), Seychelles, Palau" << endl;
		cout << "  - Bahamas, parts of Bangladesh, Miami area, New Orleans" << endl;
		cout << "\nMEDIUM-HIGH RISK:" << endl;
		cout << "  - Singapore, Netherlands (low areas), Venice, Jakarta" << endl;
		cout << "  - Major delta regions (Mekong, Nile, Ganges-Brahmaputra)" << endl;
	}
	else {
		cout << "\nCRITICAL RISK (Submerged or uninhabitable):" << endl;
		cout << "  - Maldives, Tuvalu, Marshall Islands, Kiribati, Nauru" << endl;
		cout << "  - Cook Islands, Tokelau, Micronesia (low-lying areas)" << endl;
		cout << "  - Most atoll nations in Pacific and Indian Oceans" << endl;
		cout << "\nHIGH RISK (Massive displacement and infrastructure loss):" << endl;
		cout << "  - All remaining Pacific island nations" << endl;
		cout << "  - Bahamas, Seychelles, Mauritius" << endl;
		cout << "  - Large portions of Bangladesh, Vietnam, Myanmar" << endl;
		cout << "  - Miami, New Orleans, Alexandria, Jakarta, Shanghai (coastal areas)" << endl;
		cout << "\nMEDIUM-HIGH RISK (Major coastal cities affected):" << endl;
		cout << "  - New York, London, Tokyo, Mumbai, Dhaka, Lagos, Manila" << endl;
		cout << "  - Amsterdam, Venice, Copenhagen, Bangkok, Ho Chi Minh City" << endl;
	}
}

// Display menu and get user choice
int displayMainMenu() {
	cout << "\n\n========================================" << endl;
	cout << "  CLIMATE CHANGE ADAPTOR" << endl;
	cout << "  Sea Level Rise Analysis System" << endl;
	cout << "========================================" << endl;
	cout << "\n1. Calculate sea level rise by year" << endl;
	cout << "2. Analyze ice melt impact for a region" << endl;
	cout << "3. Calculate impacts for specific region" << endl;
	cout << "4. Compare all scenarios" << endl;
	cout << "5. View islands at risk timeline" << endl;
	cout << "6. List all available regions" << endl;
	cout << "7. Regional vulnerability assessment" << endl;
	cout << "0. Exit" << endl;
	cout << "\nEnter your choice: ";

	int choice;
	cin >> choice;
	return choice;
}

// List all available regions
void listAllRegions(const vector<Region>& regions) {
	cout << "\n=== Available Regions in Database ===" << endl;
	cout << "\nBy Vulnerability Level:" << endl;

	vector<string> categories = {"Critical", "High", "Medium", "Low"};

	for (const string& category : categories) {
		cout << "\n" << category << " Vulnerability:" << endl;
		int count = 0;
		for (const Region& region : regions) {
			if (region.vulnerability == category) {
				cout << "  - " << region.name;
				if (region.areaSqKm < 1000) {
					cout << " (Small island/territory)";
				}
				cout << endl;
				count++;
			}
		}
		if (count == 0) {
			cout << "  None" << endl;
		}
	}

	cout << "\nTotal regions in database: " << regions.size() << endl;
}

// Vulnerability assessment
void vulnerabilityAssessment(const vector<Region>& regions) {
	cout << "\n=== Global Vulnerability Assessment ===" << endl;

	double totalPopAtRisk = 0;
	double totalGDPAtRisk = 0;
	int criticalRegions = 0;
	int highRiskRegions = 0;

	for (const Region& region : regions) {
		if (region.vulnerability == "Critical") {
			totalPopAtRisk += region.populationMillions * (region.coastalPopulationPercent / 100.0);
			totalGDPAtRisk += region.gdpBillions * (region.coastalPopulationPercent / 100.0) * 0.5;
			criticalRegions++;
		} else if (region.vulnerability == "High") {
			totalPopAtRisk += region.populationMillions * (region.coastalPopulationPercent / 100.0) * 0.7;
			totalGDPAtRisk += region.gdpBillions * (region.coastalPopulationPercent / 100.0) * 0.3;
			highRiskRegions++;
		}
	}

	cout << "\nGlobal Summary:" << endl;
	cout << "  Critical vulnerability regions: " << criticalRegions << endl;
	cout << "  High vulnerability regions: " << highRiskRegions << endl;
	cout << "  Total population at significant risk: " << fixed << setprecision(1)
	     << totalPopAtRisk << " million" << endl;
	cout << "  Total GDP at risk: $" << setprecision(1) << totalGDPAtRisk << " billion" << endl;

	cout << "\n\nMost Vulnerable Regions (by elevation):" << endl;
	vector<Region> sortedRegions = regions;
	sort(sortedRegions.begin(), sortedRegions.end(),
	     [](const Region& a, const Region& b) { return a.avgElevation < b.avgElevation; });

	for (int i = 0; i < min(10, (int)sortedRegions.size()); i++) {
		cout << "  " << (i+1) << ". " << sortedRegions[i].name
		     << " (avg elevation: " << sortedRegions[i].avgElevation << "m)" << endl;
	}
}

int main()
{
	cout << fixed << setprecision(2);

	// Initialize data
	vector<SeaRiseScenario> scenarios = initializeScenarios();
	vector<Region> regions = initializeRegions();

	cout << "Climate Change Adaptor initialized with:" << endl;
	cout << "  - " << scenarios.size() << " climate scenarios" << endl;
	cout << "  - " << regions.size() << " regions/countries in database" << endl;
	cout << "  - Data based on IPCC AR6 projections" << endl;

	int choice;
	do {
		choice = displayMainMenu();

		switch(choice) {
			case 1: {
				// Calculate sea level rise
				int year;
				cout << "\nEnter target year (2025-2150): ";
				cin >> year;

				if (year < 2025 || year > 2150) {
					cout << "Invalid year. Please enter a year between 2025 and 2150." << endl;
					break;
				}

				cout << "\n=== Sea Level Rise Projections for " << year << " ===" << endl;
				for (const SeaRiseScenario& scenario : scenarios) {
					double rise = calculateSeaRise(year, scenario);
					cout << "\n" << scenario.name << ":" << endl;
					cout << "  Rise from 2025: " << rise << " cm (" << rise/2.54 << " inches)" << endl;
					cout << "  Cumulative rise from pre-industrial: " << rise + 23.0 << " cm" << endl;
				}
				break;
			}

			case 2: {
				// Ice melt impact
				cout << "\nEnter region name (or part of name): ";
				cin.ignore();
				string searchName;
				getline(cin, searchName);

				bool found = false;
				for (const Region& region : regions) {
					if (region.name.find(searchName) != string::npos ||
					    searchName.find(region.name) != string::npos) {
						calculateIceMeltImpact(region);
						found = true;
						break;
					}
				}

				if (!found) {
					cout << "Region not found. Use option 6 to see all available regions." << endl;
				}
				break;
			}

			case 3: {
				// Impact analysis
				cout << "\nEnter region name: ";
				cin.ignore();
				string searchName;
				getline(cin, searchName);

				cout << "\nSelect scenario:" << endl;
				for (size_t i = 0; i < scenarios.size(); i++) {
					cout << (i+1) << ". " << scenarios[i].name << endl;
				}
				cout << "Choice: ";
				int scenarioChoice;
				cin >> scenarioChoice;

				if (scenarioChoice < 1 || scenarioChoice > (int)scenarios.size()) {
					cout << "Invalid scenario choice." << endl;
					break;
				}

				cout << "\nEnter target year: ";
				int year;
				cin >> year;

				bool found = false;
				for (const Region& region : regions) {
					if (region.name.find(searchName) != string::npos ||
					    searchName.find(region.name) != string::npos) {
						double rise = calculateSeaRise(year, scenarios[scenarioChoice-1]);
						calculateImpacts(region, rise);
						found = true;
						break;
					}
				}

				if (!found) {
					cout << "Region not found." << endl;
				}
				break;
			}

			case 4: {
				// Compare scenarios
				cout << "\nEnter target year for comparison: ";
				int year;
				cin >> year;

				cout << "\n=== Scenario Comparison for " << year << " ===" << endl;
				cout << "\n" << setw(25) << left << "Scenario"
				     << setw(15) << "Rise (cm)"
				     << setw(15) << "Rise (inches)" << endl;
				cout << string(55, '-') << endl;

				for (const SeaRiseScenario& scenario : scenarios) {
					double rise = calculateSeaRise(year, scenario);
					cout << setw(25) << left << scenario.name
					     << setw(15) << rise
					     << setw(15) << rise/2.54 << endl;
				}
				break;
			}

			case 5: {
				// Islands at risk
				cout << "\nEnter year to view islands at risk: ";
				int year;
				cin >> year;
				getIslandsAtRisk(year);
				break;
			}

			case 6: {
				// List regions
				listAllRegions(regions);
				break;
			}

			case 7: {
				// Vulnerability assessment
				vulnerabilityAssessment(regions);
				break;
			}

			case 0:
				cout << "\nThank you for using Climate Change Adaptor." << endl;
				cout << "Together we can work towards climate resilience." << endl;
				break;

			default:
				cout << "\nInvalid choice. Please try again." << endl;
		}

	} while (choice != 0);

	return 0;
}




 